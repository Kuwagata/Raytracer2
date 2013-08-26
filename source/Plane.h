#include <math.h>
#include "Object.h"

/* Defines an infinite Plane. */
class Plane : public Object {
   private:
      /* specifies the distance from the origin of the Plane. */
      double distance;

      /* stores the unit-length normal to specify the orientation of the Plane */
      Point normal;
      Color ambient, diffuse;

   public:
      Plane (double, const Point &, const Color &, const Color &);
      Plane (std::ifstream &in);
      virtual Color get_ambient (const Point &) const;
      virtual Color get_diffuse (const Point &spot) const { 
         return diffuse;
      }
      virtual bool get_intersect(const Point &sp, const Point &dp,
                         intersection &intersect);
      virtual Point get_normal (const Point &) const {
         return normal;
      }
 
      friend std::ostream& operator<< (std::ostream &out, const Plane &) {
         return out << "Plane";
      }
};
