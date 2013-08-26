#include "Raytracer.h"

float Raytracer::DIST_WEIGHT = .4f;
double Raytracer::MAX_DIST = .2;

Raytracer::Raytracer (PhotonMap &mp, bool mo, int w, int h) : map_only(mo), 
                      map (mp), scene(map.get_scene()),
                      width (w), height(h) {
  image = new unsigned char [width*height*CHANNELS];
}
  
void Raytracer::create_image (void) {
   trace();
   output();
}

Color Raytracer::antialias_trace (const Point &sp, int row, int col, 
                                 double traveled) const {
   // As we trace each pixel, we determine the direction the pixel is from 
   //    the eye point. The direction defines the ray that we are tracing. 
   //    Direction computation is introduced in version 1.0.
   Point dir = Scene::virtual_coord (row, col, height, width);

   // In version 1.0 actual tracing of the ray occurs, starting at the eye 
   //    point, toward the computed direction, using the defined scene. 
   //    Beginning in version 1.0, the Color class is user in order to all 
   //    the Colors to be computed in the range of [0, 1]. After color  
   //    computation is complete, colors are scaled into the [0,255] range. 
   Point::refractindex = 1.0;
   Color colorSum = trace_pixel (sp, dir, traveled);
   
   for (int i=1; i < NUM_TRACES; ++i) {
      dir = Scene::virtual_coord ((int)(row-.5+jitter(2,i)), 
                                  (int)(col-.5+jitter(3,i)), height, width);
      Point::refractindex = 1.0;
      colorSum += trace_pixel (sp, dir, traveled);
   }
   
   return colorSum /= NUM_TRACES;
}
 
void Raytracer::output (void) const {
   std::cout << "P6" << endl << width << " " << height << endl << "255\n";
   std::cout.write ((char *)image, width * height * CHANNELS);
}

void Raytracer::add_diffuse (Color &pixel, 
                             const intersection &intersect) const {
   const Light *light = scene.next_light_visible(&intersect.spot);
   Point unit_dir;
   double dist, weight;
   Object *obj = intersect.obj;

   while (light) {
      unit_dir = intersect.spot.get_unit_vector (light->location());
      dist = intersect.spot.distance (light->location());
      weight = obj->get_normal (intersect.spot).dot_prod(unit_dir);

      weight /= dist*Point::DIST_WT;

      pixel += obj->get_diffuse(intersect.spot) * 
             light->get_ambient(intersect.spot) * weight;
      light = scene.next_light_visible (NULL);
   }
}

// Broken
Color Raytracer::compute_transmission (const intersection &intersect, 
                         const Point &sp, double traveled) const {

   Point normal = intersect.obj->get_normal(intersect.spot);

   // get the unit vector from the hit spot to the starting point
   Point incoming = sp.get_unit_vector (intersect.spot);
   Point outgoing = incoming.bend (normal, intersect.obj->refractionindex());
   //Point outgoing = incoming;

   // Add the hit spot to the outgoing angle so it can be the direction.
   outgoing += intersect.spot;

   Color trans = trace_pixel (intersect.spot, outgoing, traveled);
   //if (trans != Color::black()) std::cerr << "NON ZERO: " << trans << std::endl;
   return trans;

}

Color Raytracer::compute_specular (const intersection &intersect,
                         const Point &sp, double traveled) const {

   Point normal = intersect.obj->get_normal(intersect.spot);
         
   // get the unit vector from the hit spot to the starting point
   Point incoming = intersect.spot.get_unit_vector (sp);

   // bounce the angle around the normal
   Point outgoing = incoming.bounce (normal);      
      
   // Add the hit spot to the outgoing angle so it can be the direction.
   outgoing += intersect.spot;
   
   // compute specular by tracing the bounce
   return trace_pixel (intersect.spot, outgoing, traveled);
}

Color Raytracer::trace_pixel (const Point &sp, const Point &dp, 
                                        double traveled) const {
   static const Color BLACK;
   if (traveled > Point::MAX) return BLACK;
   intersection intersect = scene.first_visible (sp, dp);
   Color pixel = BLACK;
   Color global_illum = BLACK;

   if ((intersect.obj->reflectivity().get_red()   < .1 || 
       intersect.obj->reflectivity().get_green() < .1 ||
       intersect.obj->reflectivity().get_blue()  < .1) &&
       intersect.obj->refractivity() < .1) {
      global_illum = map.illumination(intersect.spot, 
                           intersect.obj->get_normal (intersect.spot), MAX_DIST, 
                           NUM_PHOTONS);
   }

   pixel = intersect.obj->get_ambient(intersect.spot);
   if (map_only) { 
      return global_illum; 
   } else if (!intersect.obj->allows_lighting()) {
      return intersect.obj->get_ambient(intersect.spot);
   } else {

      add_diffuse (pixel, intersect);

      traveled += sp.distance (intersect.spot);
      pixel /= DIST_WEIGHT * traveled;
      pixel *= (intersect.obj->reflectivity()+
               intersect.obj->refractivity()).complement();

      Color specular, transmission;
      if (intersect.obj->reflectivity().get_red()   > Point::MIN || 
          intersect.obj->reflectivity().get_green() > Point::MIN ||
          intersect.obj->reflectivity().get_blue()  > Point::MIN) {
         specular = compute_specular (intersect, sp, traveled);
         specular *=  intersect.obj->reflectivity();
         pixel += specular;
      }
      if (intersect.obj->refractivity() > Point::MIN) {
         transmission = compute_transmission (intersect, sp, traveled);
         transmission *= intersect.obj->refractivity();
         pixel += transmission;
      }
   }

   pixel += global_illum;
   return pixel;
}

void Raytracer::trace (void) {
   time_t start, end;
   Color color;
   double diff;

   time (&start);
   std::cerr << "Row " << std::setw(3) << '0';
   for (int i=0; i < height; ++i) {
      time (&end);
      diff = difftime (end, start);
      if (diff > 0) {
         time (&start);
         std::cerr << "\b\b\b" << std::setw(3) << i;
      }

      for (int j=0; j < width; ++j) {
         color = antialias_trace (scene.eye_pt(), i, j, 0.0); 
         color.to_byte_range ();
         for (int k=RED; k <=BLUE; ++k) {
            image[(i*width +j)*CHANNELS + k] = (unsigned char)color[k];
         }
      }
   } 
   std::cerr << "\b\b\b" << std::setw(3) << height;
}

double Raytracer::jitter (int base, int number) {
   double radical_inverse = 0.0;
   double digit_place = 1.0/base;

   while (number > 0) {
      radical_inverse += digit_place * (number%base);
      number /= base;
      digit_place *= 1.0/base;
   }
   return radical_inverse;
}

int main (int argc, char *argv[]) {
  time_t start,end;
  double dif;

  time (&start);
   if (1 == argc) {
      std::cerr << "Usage: " << argv[0] << " input file [width height]"
                << " > out.ppm" << std::endl;
      return EXIT_FAILURE;
   }

   Raytracer *tracer;
   Scene scene (argv[1]);
   PhotonMap map (scene);
   map.trace_photons();
   if (argc > 3) {
      tracer = new Raytracer (map, Raytracer::MAP_ONLY, atoi(argv[2]), atoi(argv[3]));
   } else {
      tracer = new Raytracer (map, Raytracer::MAP_ONLY);
   }
   tracer->create_image();
   time (&end);
   dif = difftime (end,start);
   float min = dif/60;
   std::cerr << endl << min << " minutes" << endl;
   delete tracer;
}

