#ifndef OBJECTCLASS
#define OBJECTCLASS
#include <iostream>
#include <fstream>
#include "Color.h"
#include "Point.h"

class Object;

/* This structure stores a pointer to the Object intersected, the t distance
 * value to the Object intersected, and the x,y,z value of the location 
 * at which the Object was intersected.
 */
typedef struct {
   Object *obj;
   double t;
   Point spot;
} intersection;

/* Generic base class specifying attributes needed for inheriting objects */
class Object {
   protected:
      /* reflectwt is the weight [0.0, 1.0] that reflective lighting has on the 
       * color of this object.
       */
      Color reflectwt;

      /* refractwt is the weight [0.0, 1.0] that refractive lighting has on the
       * color of this object.
       * refractindex is the index of refractivity used with Moore's Law to 
       * compute the angle at which light should be bent.
       */
      double refractwt, refractindex;
   public:
      Object () : reflectwt (0.0,0.0,0.0), refractwt (0.0), refractindex(1.0) {}
      Object (const Color &r) : reflectwt (r), refractwt (0.0), 
                                refractindex(1.0) {}
      virtual ~Object () {} // always make destructor virtual for parent classes

      /* returns the ambient color for this Object. For objects that do not have
       * lighting effects (Sky, Light), this is the only color value available.
       */
      virtual Color get_ambient (const Point &spot) const = 0;

      /* returns the diffuse color for this Object. */
      virtual Color get_diffuse (const Point &spot) const = 0;
      const Color &reflectivity () const { return reflectwt; }
      double refractivity () const { return refractwt; }
      double refractionindex () const { return refractindex;}
      virtual bool get_intersect(const Point &sp, const Point &dp, 
                               intersection &intersect) =0;
      /* Specifies whether this Object reacts to specular or diffuse lighting. */
      virtual bool allows_lighting (void) const { return true; }

      /* Returns a unit-length normal at the given location. */
      virtual Point get_normal (const Point &) const = 0;
      friend std::ostream &operator<< (std::ostream &out, const Object &obj) {
         return out << "object: " << obj.reflectwt.get_red() << ", " 
                << obj.reflectwt.get_green() << ", " << obj.reflectwt.get_blue();
      }
};
#endif
