#include "Object.h"
#include <math.h>

/* Defines a sphere object */
class Sphere : public Object {
   protected:
      double radius;
      Point center;
      Color ambient, diffuse;
   public:
      Sphere () : Object () {} // calls the Object's default constructor
      Sphere (double, const Point &, const Color &, const Color &);
      Sphere (std::ifstream &);
      virtual const Point &location () const { return center; }
      virtual Color get_ambient (const Point &) const;
      virtual Color get_diffuse (const Point &spot) const {
         return diffuse;
      }
      virtual bool get_intersect(const Point &sp, const Point &dp, 
                               intersection &intersect);
      virtual Point get_normal (const Point &pt) const {
         return center.get_unit_vector (pt);
      }
      friend std::ostream& operator<< (std::ostream &out, const Sphere &) {
         return out << "Sphere";
      }
};
