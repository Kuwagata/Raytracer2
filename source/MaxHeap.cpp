#include "MaxHeap.h"

MaxHeap::MaxHeap(int cap) : size(0), capacity(cap)
{
   array = new Closest[capacity+1];
}

MaxHeap::~MaxHeap()
{
   if(array != NULL)
   {
      delete[] array;
   }
   array = NULL;
}

bool MaxHeap::isFull() const
{
   return size == capacity;
}

int MaxHeap::get_size() const
{
   return size;
}

void MaxHeap::clear()
{
   size = 0;
}
      
void MaxHeap::insert(const Closest &toIns)
{
   isFull() ? insertFull(toIns) : insertNotFull(toIns);
}

void MaxHeap::insertFull(const Closest &toIns)
{
   /* Compare to top of heap;  If top is larger, delete and percolate down */
   if(findMax().distance > toIns.distance)
   {
      //percolate down
      int hole = 1;
      int largerChild;
      bool hasLeft = (2 * hole) <= size;
      bool hasRight = (2 * hole + 1) <= size;
      
      while(hasLeft && (array[hole*2] > toIns ||
      (hasRight && array[hole*2+1] > toIns)))
      {
         largerChild = (hasRight && array[hole*2+1] > array[hole*2]) ? (hole * 2 + 1) : (hole * 2);
         
         array[hole] = array[largerChild];
         hole = largerChild;
         
         hasLeft = (2 * hole) <= size;
         hasRight = (2 * hole + 1) <= size;
      }
      
      array[hole] = toIns;
   }
}

void MaxHeap::insertNotFull(const Closest &toIns)
{
   /* Create hole and percolate up */
   int hole = ++size;
   
   while(hole > 1 && array[hole/2] < toIns)
   {
      array[hole] = array[hole/2];
      hole /= 2;
   }
   
   array[hole] = toIns;
}

void MaxHeap::delMax()
{
   Closest toIns = array[size];
   int hole = 1;
   int largerChild;
   bool hasLeft = (2 * hole) <= size;
   bool hasRight = (2 * hole + 1) <= size;
   
   while(hasLeft && (array[hole*2] > toIns ||
   (hasRight && array[hole*2+1] > toIns)))
   {
      largerChild = (hasRight && array[hole*2+1] > array[hole*2]) ? (hole * 2 + 1) : (hole * 2);
      
      array[hole] = array[largerChild];
      hole = largerChild;
      
      hasLeft = (2 * hole) <= size;
      hasRight = (2 * hole + 1) <= size;
   }
   
   array[hole] = toIns;
   size--;
}

const Closest &MaxHeap::findMax() const
{
   return array[1];
}

Color MaxHeap::sumColors() const
{
   Color toReturn;
   
   for(int i = 1; i <= size; i++)
   {
      toReturn += (array[i]).closest->rgb;
   }
   
   return toReturn;
}
