#include "Sky.h"

Sky::Sky (std::ifstream &in) : Object (){
   std::string attribute;
   // loop 3 times for 3 attributes
   for (int i=0; i < 3; ++i) {
      in >> attribute;
      if (attribute == "horizon") {
         in >> horizon;
      } else if (attribute == "base") {
         in >> base;
      } else if (attribute == "blue") {
         in >> blue;
      }
   }
}

Sky::Sky (double hor, double ba, double bl) : Object (),
          base (ba), horizon (hor), blue (bl){
}

Color Sky::get_ambient (const Point &spot) const {
   double redgreen = (1-spot[Y])*horizon + base;
   return Color(redgreen, redgreen, blue);
}

bool Sky::get_intersect(const Point &sp, const Point &dp, 
                        intersection &intersect) {
   intersect.t = Point::MAX;
   intersect.spot = sp.get_unit_vector(dp);
   intersect.obj = this;
   return true;
}

