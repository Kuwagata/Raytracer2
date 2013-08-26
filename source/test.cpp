#include <iostream>

using std::cout;
using std::endl;

int main (int argc, char *argv[]) {
   const unsigned int TOTAL_PHOTONS = 30;
   double max_dist = 100;
   Point spot(0,0,0);
   
   Photon photons[TOTAL_PHOTONS];
   
   Point max(1, 1, 1);
   Point min(-1, -1, -1);
   Color color (1.0/TOTAL_PHOTONS, 1.0/TOTAL_PHOTONS, 1.0/TOTAL_PHOTONS);
   
   for (unsigned int i = 0; i < TOTAL_PHOTONS; ++i) {
      photons[i].rgb = color;
      
      for (int j = 0; j < 3; ++j) {
         photons[i].loc[j] = (rand() / (double) RAND_MAX) * (max[j] - min[j]) + min[j];
      }
   }
   
   MaxHeap heap(10);
   
   for(unsigned int i = 0; i < TOTAL_PHOTONS; i++) {
         Point diff = spot - photons[i].loc;
         double sqDist = diff.dot_prod(diff);
         
         if( sqDist < squareMaxDist) {
            Closest toAdd (&photons[i], sqDist);
            clsPhotons.insert(toAdd);
         }
   }
}
