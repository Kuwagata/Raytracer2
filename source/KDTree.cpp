#include "KDTree.h"
#include <iostream>

/* KDTree Private Methods */
void KDTree::insert (int position, KDNode* &toIns)
{
   array[position] = toIns;
   return;
}

// Return number of elements found within maxDist and added to the heap, h
int KDTree::locate (int curIndex, MaxHeap* &h, double &maxDist, const Point &target) const
{
   int left = 2 * curIndex;
   int right = 2 * curIndex+1;
   int count = 0;

   // We're at the bottom of the tree - no points
   if(curIndex * 2 > size)
   {
      return 0;
   }
   else
   {
      double curDist = distCalc(target, array[curIndex]->p);

      // If the point is within range
      if(curDist < maxDist)
      {
         //insert and search left + right
         Closest toAdd(array[curIndex]->p, curDist);
         h->insert(toAdd);
         count++;
         count += locate(left, h, maxDist, target);
         count += locate(right, h, maxDist, target);
      }
      else
      {
         // Get the diff on this depth's axis
         double diff = target[array[curIndex]->axis] - array[curIndex]->p->loc[array[curIndex]->axis];
         diff *= diff;

         // If less, eligible points could exist in either subtree
         if(diff < maxDist)
         {
            //search left and right
            count += locate(left, h, maxDist, target);
            count += locate(right, h, maxDist, target);
         }
         else
         {
            // Eligible points exist only in the left or right subtree
            if(target[array[curIndex]->axis] <= array[curIndex]->p->loc[array[curIndex]->axis])
            {
               //search left
               count += locate(left, h, maxDist, target);
            }
            else
            {
               //search right
               count += locate(right, h, maxDist, target);
            }
         }
      }
   }
   return count;
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

/*
 * Construct at the full range - 0 -> num photons
 * Split list in half
 */
void KDTree::insert(Photon** &genPhotons, int left, int right, int pos)
{
   // Recursive base case - we're at a leaf
   if(left == right)
   {
      KDNode* newnode = new KDNode(genPhotons[left], pos);
      insert(pos, newnode);
      return;
   }

   // Split list in half
   int target = (left + right) / 2;
   double target_full = ((double) left + (double) right) / 2.0;

   // Find best axis to categorize on for the subset of points constrained by left, right
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
   // Start looking at the root
   int numFound = locate(1, toReturn, dist, target);
   //std::cerr << "Found " << numFound << " photons; Heap contains " << toReturn->get_size() << std::endl;
   return toReturn;
}

// Determine which axis to classify on (i.e. does this level of the tree classify on x, y, or z?)
int KDTree::findAxis(Photon** &genPhotons, int left, int right)
{
   // Start at first photon in the list, set default min, max = min at beginning
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
   // base case - we're at an empty leaf
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

// simple swap
void KDTree::swaptwo(Photon** &genPhotons, int a, int b)
{
   Photon* temp;
   temp = genPhotons[a];
   genPhotons[a] = genPhotons[b];
   genPhotons[b] = temp;
}

// in-place sort of three photons based on supplied axis
// negative to positive
void KDTree::swapthree(Photon** &genPhotons, int axis, int x, int y, int z)
{
   Photon * a;
   Photon * b;
   Photon * c;
   a = genPhotons[x];
   b = genPhotons[y];
   c = genPhotons[z];

   // if x > y & z
   if((a->loc[axis] > b->loc[axis])&&(a->loc[axis] > c->loc[axis]))
   {
      swaptwo(genPhotons, x, z);
   }

   // if x > y
   if(a->loc[axis] > b->loc[axis])
   {
      swaptwo(genPhotons, x, y);
   }
}
