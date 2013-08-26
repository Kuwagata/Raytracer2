#include "Box.h"

Box::Box (std::ifstream &in) {
   std::string attribute;
   // loop 6 times for 6 attributes
   for (int i=0; i < 6; ++i) {
      in >> attribute;
      if (attribute == "min") {
         in >> left_low_far;
      } else if (attribute == "max") {
         in >> right_high_near;
      } else if (attribute == "color") {
         in >> diffuse;
         ambient = diffuse * Color::AMB_WT;
      } else if (attribute == "reflectwt") {
         in >> reflectwt;
      } else if (attribute == "refractwt") {
         in >> refractwt;
      } else if (attribute == "refractindex") {
         in >> refractindex;
      }
   }
}


Box::Box(const Point &min, const Point &max, const Color &c, const Color &r)
        : Object (r), left_low_far(min), right_high_near(max), 
        ambient(c*Color::AMB_WT), diffuse(c){}

Color Box::get_ambient (const Point &spot) const {
   return ambient;
}

bool Box::get_intersect(const Point &sp, const Point &dp,
                          intersection &intersect) {

   double tnear = -Point::MAX;
   double tfar  =  Point::MAX;
   double t1, t2;
   
   for (int i=X; i <= Z; ++i) {
      if (equals(sp[i], dp[i])) {
         if (sp[i] < left_low_far[i] || sp[i] > right_high_near[i]) {
            return false;
         }
      } else {
      
         // find the distance to the two points along x.
         t1 = (left_low_far[i]    - sp[i]) / (dp[i] - sp[i]);
         t2 = (right_high_near[i] - sp[i]) / (dp[i] - sp[i]);

         // make sure t1 is holding the distance to the closer plane.
         if (t1 > t2) swap (t1, t2);
   
         // store the closest distances
         if (t1 > tnear) { tnear = t1; }
         if (t2 < tfar) { tfar = t2; }
         
         if (tnear > tfar || tfar < 0.0) return false; 
      }
   }
   
   if (tnear < Point::MIN) return false;
   
   intersect.spot  = sp * (1-tnear);
   intersect.spot += dp * tnear;
   intersect.t = tnear;
   intersect.obj = this;
   return true;
}


Point Box::get_normal (const Point &spot) const {
   Point normal (0,0,0);
   for (int i=X; i <=Z; ++i) {
      if (equals (spot[i], left_low_far[i])) {
         normal[i] = -1;
         return normal;
      }
   }
   for (int i=X; i <=Z; ++i) {
      if (equals (spot[i], right_high_near[i])) {
         normal[i] = 1;
         return normal;
      }
   }
   return normal;
}

void swap (double &d1, double &d2) {
   static double temp;
   temp = d1;
   d1 = d2;
   d2 = temp;
}
