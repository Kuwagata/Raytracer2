#include "KDTree.h"
#include <iostream>

/* KDTree Private Methods */
void KDTree::insert (int position, KDNode* &toIns)
{
   array[position] = toIns;
   return;
}

void KDTree::locate (int curIndex, MaxHeap* &h, double &maxDist, const Point &target) const
{
   int left = 2 * curIndex;
   int right = 2 * curIndex+1;
   //old: if(curIndex >= size)
   if(curIndex * 2 >= size)
   {
      return;
   }
   else
   {
      double curDist = distCalc(target, array[curIndex]->p);
      if(curDist < maxDist)
      {
         //insert and search left + right
         Closest toAdd(array[curIndex]->p, curDist);
         h->insert(toAdd);
         locate(left, h, maxDist, target);
         locate(right, h, maxDist, target);
      }
      else
      {
         double diff = target[array[curIndex]->axis] - array[curIndex]->p->loc[array[curIndex]->axis];
         diff *= diff;
         if(diff < maxDist)
         {
            //search left and right
            locate(left, h, maxDist, target);
            locate(right, h, maxDist, target);
         }
         else
         {
            if(target[array[curIndex]->axis] <= array[curIndex]->p->loc[array[curIndex]->axis])
            {
               //search left
               locate(left, h, maxDist, target);
            }
            else
            {
               //search right
               locate(right, h, maxDist, target);
            }
         }
      }
   }
}

double KDTree::distCalc(const Point &point, Photon* &photon) const
{
   Point diff = point - photon->loc;
   double sqDist = diff.dot_prod(diff);
   return sqDist;
}

/* KDTree Public Methods */
KDTree::~KDTree ()
{
   // Array delete fails, as first element points to null
   // delete []array;

   for(int i = 1; i < (size + 1); i++)
   {
      delete array[i];
   }
}

void KDTree::insert(Photon** &genPhotons, int left, int right, int pos)
{
   // Recursive base case
   if(left == right)
   {
      KDNode* newnode = new KDNode(genPhotons[left], pos);
      insert(pos, newnode);
      return;
   }

   // Target is average of left and right
   int target = (left + right) / 2;
   int axis = findAxis(genPhotons, left, right);

   // Inserts target
   median(genPhotons, left, right, target, axis, pos);

   insert(genPhotons, left, target-1, 2 * pos);
   insert(genPhotons, target+1, right, 2 * pos + 1);
   return;
}

// Generate a maxheap of nearest N photons
MaxHeap* KDTree::findNearestN (int num_photons, double &dist, const Point &target) const
{
   MaxHeap* toReturn = new MaxHeap(num_photons);
   locate(1, toReturn, dist, target);
   return toReturn;
}

// Determine which axis to classify on (i.e. does this level of the tree classify on x, y, or z?)
int KDTree::findAxis(Photon** &genPhotons, int left, int right)
{
   double xmin = genPhotons[left]->loc.x;
   double ymin = genPhotons[left]->loc.y;
   double zmin = genPhotons[left]->loc.z;
   double xmax = xmin;
   double ymax = ymin;
   double zmax = zmin;
   
   for( int i = left + 1; i <= right; i++)
   {
      if(genPhotons[i]->loc.x < xmin)
         xmin = genPhotons[i]->loc.x;
      if(genPhotons[i]->loc.y < ymin)
         ymin = genPhotons[i]->loc.y;
      if(genPhotons[i]->loc.z < zmin)
         zmin = genPhotons[i]->loc.z;
      if(genPhotons[i]->loc.x > xmax)
         xmax = genPhotons[i]->loc.x;
      if(genPhotons[i]->loc.y > ymax)
         ymax = genPhotons[i]->loc.y;
      if(genPhotons[i]->loc.z > zmax)
         zmax = genPhotons[i]->loc.z;
   }
   
   double xdif = xmax - xmin;
   double ydif = ymax - ymin;
   double zdif = zmax - zmin;
   
   if(xdif > ydif)
      if(xdif > zdif)
         return 0;
      else
         return 2;
   else
      if(ydif > zdif)
         return 1;
      else
         return 2;
}

void KDTree::median(Photon** &genPhotons, int left, int right, int target, int axis, int pos)
{
   if(right == left)
   {
      KDNode* newnode = new KDNode(genPhotons[target], axis);
      insert(pos, newnode);
      return;
   }
   if((right - left) == 1)
   {
      if(genPhotons[left]->loc[axis] > genPhotons[right]->loc[axis])
      {
         swaptwo(genPhotons, left, right);
      }
      KDNode* newnode = new KDNode(genPhotons[target], axis);
      insert(pos, newnode);
      return;
   }
   if((right - left) == 2)
   {
      swapthree(genPhotons,axis, left, left+1, left+2);
      KDNode* newnode = new KDNode(genPhotons[target], axis);
      insert(pos, newnode);
      return;
   }
   int middle = (left + right) / 2;
   swapthree(genPhotons, axis, left, middle, right);
   swaptwo(genPhotons, middle, right-1);
   int i = left; int j = right-1;
   for(;;)
   {
      while(genPhotons[++i]->loc[axis] < genPhotons[right-1]->loc[axis]) {}
      //old: while((genPhotons[--j]->loc[axis] > genPhotons[right-1]->loc[axis]) && j > (i-2)) {}
      //new: Changed i-2 to i-1 to prevent j going to -1
      while((genPhotons[--j]->loc[axis] > genPhotons[right-1]->loc[axis]) && j > (i-1)) {}
      if(i<j)
      {
         swaptwo(genPhotons, i, j);
      }
      else
      {
         break;
      }
   }
   swaptwo(genPhotons, right-1, i);
   if(i == target)
   {
      KDNode* newnode = new KDNode(genPhotons[target], axis);
      insert(pos, newnode);
      return;
   }
   if(i > target)
   {
      median(genPhotons, left, i-1, target, axis, pos);
   }
   if(i < target)
   {
      median(genPhotons, i+1, right, target, axis, pos);
   }
   return;
}

void KDTree::swaptwo(Photon** &genPhotons, int a, int b)
{
   Photon* temp;
   temp = genPhotons[a];
   genPhotons[a] = genPhotons[b];
   genPhotons[b] = temp;
}

void KDTree::swapthree(Photon** &genPhotons, int axis, int x, int y, int z)
{
   Photon * a;
   Photon * b;
   Photon * c;
   a = genPhotons[x];
   b = genPhotons[y];
   c = genPhotons[z];
   if((a->loc[axis] > b->loc[axis])&&(a->loc[axis] > c->loc[axis]))
   {
      swaptwo(genPhotons, x, z);
   }
   if(a->loc[axis] > b->loc[axis])
   {
      swaptwo(genPhotons, x, y);
   }
}
