#include "Floor.h"

Floor::Floor (std::ifstream &in) {
   std::string attribute;
   // loop 4 times for 4 attributes
   for (int i=0; i < 4; ++i) {
      in >> attribute;
      if (attribute == "height") {
         in >> height;
      } else if (attribute == "color1") {
         in >> diffuse1;
         ambient1 = diffuse1*Color::AMB_WT;
      } else if (attribute == "color2") {
         in >> diffuse2;
         ambient2 = diffuse2*Color::AMB_WT;
      } else if (attribute == "reflectwt") {
         in >> reflectwt;
      }
   }
}

Floor::Floor (double ht, const Color &c1, const Color &c2, const Color &r) : 
              Object (r), height (ht), diffuse1(c1), ambient1(c1*Color::AMB_WT),
              diffuse2(c2), ambient2(c2*Color::AMB_WT) {}

Color Floor::get_ambient (const Point &spot) const {
   int floorX = (int) floor (spot[X]);
   int floorZ = (int) floor (spot[Z]);

   return ((floorX+floorZ)%2==0)? ambient1 : ambient2;
}

bool Floor::get_intersect(const Point &sp, const Point &dp,
                          intersection &intersect) {

   if (fabs (dp[Y]-sp[Y]) < Point::MIN) return false;

   intersect.t = (height- sp[Y])/(dp[Y] - sp[Y]);
   if (intersect.t < Point::MIN) return false;

   intersect.spot[X] = sp[X] * (1.0-intersect.t) + dp[X] * intersect.t; 
   intersect.spot[Y] = height;
   intersect.spot[Z] = sp[Z] * (1.0-intersect.t) + dp[Z] * intersect.t; 
   intersect.obj = this;
   return true;
}
