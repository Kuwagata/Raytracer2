#include <iostream>
#include "PhotonMap.h"
#include <iomanip>
#include <time.h>

using std::cout;
using std::endl;

class Raytracer {

   private:
      static double MAX_DIST;
      static const int NUM_PHOTONS=800;
      static const int CHANNELS=3;
      static const int DEFAULT_WIDTH=800;
      static const int DEFAULT_HEIGHT=600;
      static float DIST_WEIGHT;
      static const int NUM_TRACES=1;
      bool map_only;

      PhotonMap &map;
      Scene &scene;
      int width, height;
      unsigned char *image;
      Color trace_pixel (const Point &, const Point &, double) const;
      void add_diffuse (Color &, const intersection &) const;
      Color compute_specular (const intersection &, const Point &, 
                                                     double) const;
      Color compute_transmission (const intersection &, const Point &, 
                                                        double) const;
      Color antialias_trace (const Point &, int, int, double) const;
      static double jitter (int base, int number);


   public:
      static const bool MAP_ONLY = true;
      Raytracer (PhotonMap &, bool map_only=false, int w=DEFAULT_WIDTH, 
                 int h=DEFAULT_HEIGHT);
      ~Raytracer (void) {
         delete []image;
      }
      void create_image(void);
      void trace (void);
      void output (void) const;
};
