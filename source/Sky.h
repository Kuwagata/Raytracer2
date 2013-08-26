#include "Object.h"

/* defines a gradient blue sky. */
class Sky : public Object {
   private:
      /* base is the minimum value for the red and green channels.
       * horizon is the maximum amount of color to add to red and green at the
       * horizon.
       * blue is the constant value of the blue channel.
       */ 
      double base, horizon, blue;

   public:
      Sky (double, double, double);
      Sky (std::ifstream &in);
      virtual Color get_ambient (const Point &) const;
      virtual Color get_diffuse (const Point &spot) const {
         return Color::black();
      }
      virtual bool get_intersect(const Point &, const Point &, intersection &);
      virtual bool allows_lighting (void) const { return false; }
      virtual Point get_normal (const Point &pt) const {
         return pt;
      }

};
