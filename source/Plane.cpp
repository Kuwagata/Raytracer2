#include "Plane.h"

Plane::Plane (std::ifstream &in) {
   std::string attribute;
   // loop 4 times for 4 attributes
   for (int i=0; i < 4; ++i) {
      in >> attribute;
      if (attribute == "distance") {
         in >> distance;
      } else if (attribute == "normal") {
         in >> normal;
         normal.normalize();
      } else if (attribute == "color") {
         in >> diffuse;
         ambient = diffuse *Color::AMB_WT;
      } else if (attribute == "reflectwt") {
         in >> reflectwt;
      }
   }
}

Plane::Plane (double dist, const Point &n, const Color &c, const Color &r) : 
              Object (r), distance (dist), normal(n), ambient(c*Color::AMB_WT), 
              diffuse(c) {}

Color Plane::get_ambient (const Point &spot) const {
   return ambient;
}

bool Plane::get_intersect(const Point &sp, const Point &dp,
                          intersection &intersect) {
   double discr;
   discr = sp.dot_prod (normal) - dp.dot_prod (normal);

   // if(P's normal.Ray's direction) = 0, the plane and the ray are parallel.
   //    t can't be negative.
   if (fabs(discr) < Point::MIN) { return false; }

   // compute the distance to the intersection
   intersect.t = (sp.dot_prod (normal)+distance)/discr;

   if (intersect.t < Point::MIN) { return false; /* behind us*/ }

   intersect.obj = this;

   // find the intersection point
   for (int i=X; i <= Z; ++i) {
      intersect.spot[i] = sp[i]*(1-intersect.t) + dp[i]*intersect.t;
   }
   return true;
}
