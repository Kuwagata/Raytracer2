#include "Light.h"

Light::Light (std::ifstream &in) {
   std::string attribute;
   reflectwt.red = reflectwt.green = reflectwt.blue = 0.0;

   // loop 3 times for 3 attributes
   for (int i=0; i < 3; ++i) {
      in >> attribute;
      if (attribute == "center") {
         in >> center;
      } else if (attribute == "radius") {
         in >> radius;
      } else if (attribute == "color") {
         in >> ambient;
      }
   }
}

Light::Light (double r, const Point &cen) : Sphere (r, cen, Color (1,1,1), 
                                                            Color(0,0,0)) {} 
