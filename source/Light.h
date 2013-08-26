#include "Sphere.h"

/* Defines the Light object to inherit from the Sphere class. */
class Light : public Sphere {
   public:
      Light (double, const Point &);
      Light (std::ifstream &in);
      const Point &location () const { return center; } 

      /* Lights are visible spheres. They do not react to other lighting
       * but instead return only ambient light. To find the color value
       * of a Light, use the get_ambient method defined in Sphere.
       */
      virtual bool allows_lighting (void) const { return false; }
      virtual Color get_diffuse (const Point &) const {
         return diffuse;
      }
      friend std::ostream& operator<< (std::ostream &out, const Light &) {
         return out << "Light";
      }
};
