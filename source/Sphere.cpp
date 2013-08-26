#include "Sphere.h"

Sphere::Sphere (std::ifstream &in) {
   std::string attribute;
   // loop 6 times for 6 attributes
   for (int i=0; i < 6; ++i) {
      in >> attribute;
      if (attribute == "center") {
         in >> center;
      } else if (attribute == "radius") {
         in >> radius;
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
// std::cerr << refractindex << std::endl;
}

Sphere::Sphere (double r, const Point &cen, const Color &col, const Color &ref):
                Object (ref), radius (r), center (cen), 
                ambient (col*Color::AMB_WT), diffuse (col) {}
Color Sphere::get_ambient (const Point &spot) const {
   return ambient;
}
bool Sphere::get_intersect(const Point &sp, const Point &dp, 
                               intersection &intersect) {
   double t;
   Point dp_minus_sp = dp - sp;
   Point sp_minus_c  = sp - center;

   double a = dp_minus_sp.dot_prod (dp_minus_sp);
   double b = 2.0 * dp_minus_sp.dot_prod (sp_minus_c);
  
   double c = sp_minus_c.dot_prod (sp_minus_c) - radius * radius;
   double discr = b*b - 4.0 * a * c;
  
   if (discr >= 0) {
      t = (-b - sqrt(discr))/(2.0*a);
    
      if (t < Point::MIN) {
         t = (-b + sqrt (discr))/(2.0*a);
      }

      if (fabs(t) < Point::MIN) return false;

      if (t > Point::MIN) {
         for (int i=X; i <= Z; ++i) {
            intersect.spot[i] = sp[i]*(1-t)+ dp[i]*t;
         }
         intersect.t = t;
         intersect.obj = this;
         return true;
      }
   }
   return false;
}
