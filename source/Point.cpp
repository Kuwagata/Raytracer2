#include "Point.h"

double Point::MIN = .000001;
double Point::DIST_WT = .1;
double Point::refractindex = 1.0;
double Point::air_refractindex = 1.0;

bool equals (double a, double b) {
   return (fabs (a-b) < Point::MIN);
}

Point::Point (double x_in, double y_in, double z_in): 
              x(x_in), y(y_in), z(z_in) {}
double Point::dot_prod (const Point &pt) const {
   return x * pt.x + y * pt.y + z * pt.z;
}
Point Point::operator/ (double divisor) const {
   return Point (x/divisor, y/divisor, z/divisor);
}

Point Point::operator- (const Point &pt) const {
   return Point (x-pt.x, y-pt.y, z-pt.z);
}
   
bool Point::operator!= (const Point &pt) const {
   return fabs (x-pt.x) >= MIN && fabs (y-pt.y) >= MIN &&
          fabs (z-pt.z) >= MIN;
}

double Point::distance (const Point &pt) const {
   Point diff = *this - pt;
   return sqrt (diff.dot_prod (diff));
}

Point Point::get_unit_vector (const Point &dp) const {
   double dist = distance (dp);
   
   if (fabs(dist) < MIN) {
    // if the distance is 0, return a 0,0,0 vector
      return zero();
   } else {
      // Find the difference in points and divide by the distance between them
      //    to get the unit vector.
      Point diff = dp - *this;
      return diff/dist;
   }
}

Point &Point::normalize () {
   double dist = sqrt (this->dot_prod(*this));
   if (fabs(dist) < MIN) {
      *this = zero();
   } else {
      *this /= dist;
   }
   return *this;
}

Point Point::bounce (const Point &normal) const {
   Point outgoing = normal * (2 * dot_prod (normal));
   return outgoing -= *this; 
}

// Should be called on a normalized, incoming vector toward intersect point.
Point Point::bend (const Point &normal, double index) const{

   double ratio, ratioSq;
   // std::cerr << *this << ": " << normal << std::endl;
   Point out;
   Point u = (*this);
   Point n = -normal;

   // exiting. Make index be the air index, reverse vectors
   if (equals(refractindex, index)) {
      // in the object and leaving
      index = air_refractindex;
      n = -n;
   }

   // std::cerr << *this << ": " << normal << std::endl;
   double dotprod = fabs(u.dot_prod(n));
   double dpSq = SQUARE(dotprod);

   ratio = refractindex/index;
   ratioSq = SQUARE (ratio);
   out = (sqrt(1-ratioSq+ratioSq*dpSq)-ratio*dotprod)*n + ratio*(u);
   return out;
}

Point Point::operator* (double mult) const {
   return Point (x*mult, y*mult, z*mult);
}

const Point &Point::operator-= (const Point &sub) {
   for (int i=X; i <=Z; ++i) {
      (*this)[i] -= sub[i];
   }
   return *this;
}

const Point &Point::operator+= (const Point &add) {
   for (int i=X; i <=Z; ++i) {
      (*this)[i] += add[i];
   }
   return *this;
}

