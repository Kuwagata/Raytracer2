#include <math.h>
#include "Object.h"

/* class defining a box object that is lined up with the axes. */

class Box : public Object {
   private:
      Point left_low_far;
      Point right_high_near;
      Color ambient, diffuse;

   public:
      Box (const Point &, const Point &, const Color &, const Color &);
      /* constructor for reading in the Box specification */
      Box (std::ifstream &in);
      virtual Color get_ambient (const Point &) const;
      virtual Color get_diffuse (const Point &spot) const { 
         return diffuse;
      }
      virtual bool get_intersect(const Point &sp, const Point &dp,
                         intersection &intersect);
      virtual Point get_normal (const Point &) const;
      friend std::ostream& operator<<(std::ostream &out, const Box &) {
         return out << "Box";
      }
};

void swap (double &d1, double &d2);
