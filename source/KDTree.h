#include "MaxHeap.h"
#include <iostream>

class KDTree
{
   private:
      class KDNode
      {
         public:
            Photon* p;
            int axis;

            KDNode(Photon* &newPhoton, int comp = 0) : p(newPhoton), axis(comp) {}
            ~KDNode() { delete p; }
      };
      
      double size;
      KDNode** array;
      
      void insert(int curIndex, KDNode* &toIns);
      int locate(int curIndex, MaxHeap* &h, double &maxDist, const Point &target) const;
      
   public:
      double distCalc(const Point &point, Photon* &photon) const;
      
      KDTree(double numPhotons) : size(numPhotons)
      {
         array = new KDNode*[(int) (size+1)];
         for(int i = 0; i <= numPhotons; i++)
            array[i] = NULL;
      }
      
      ~KDTree();
      void insert(Photon** &genPhotons, int left, int right, int pos);
      MaxHeap* findNearestN (int num_photons, double &dist, const Point &target) const;
      int findAxis(Photon** &genPhotons, int left, int right);
      void median(Photon** &genPhotons, int left, int right, int target, int axis, int pos);
      void swaptwo(Photon** &genPhotons, int a, int b);
      void swapthree(Photon** &genPhotons, int axis, int x, int y, int z);
};
