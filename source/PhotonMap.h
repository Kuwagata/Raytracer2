#include <iostream>
#include <cstdlib>
#include "Scene.h"
#include <cmath>
#include "KDTree.h"

using std::cout;

class PhotonMap
{
   private:
      Scene &scene;
      static const unsigned int TOTAL_PHOTONS = 524287;     /*2^18 = 262143*/    /*32767*/
      KDTree *kdtree;                                       /*2^19 = 524287*/
      
   public:
      // Constructor initializing the reference to the Scene. 
      PhotonMap (Scene &sc) : scene(sc) {kdtree = new KDTree(TOTAL_PHOTONS);}
      
      //Destructor may be necessary**********************
      ~PhotonMap();
      
      
      // compute brightness based on placed photons
      Color illumination (const Point &spot, const Point &normal,
                          double max_dist, unsigned int num_photons) const;
      // randomly place photons
      void trace_photons (void);
      bool trace(intersection inter, Point sp, Color photon_color, Photon** &genPhotons, unsigned int &index);
      Scene &get_scene (void) { return scene; }
};
