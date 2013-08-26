#include <stdlib.h>
#include <fstream>
#include <string>
#include "ObjList.h"
#include "Sky.h"
#include "Floor.h"
#include "Box.h"
#include "Plane.h"

/* Defines the Scene to be rendered: world coordinates WIDTH and HEIGHT and the
 * linked list of Objects and Lights.
 * Additionally, Scene provides the first_visible method for finding the closest
 * intersection, the first_object method for finding the closest non-light 
 * intersection, and virtual_coord for performing pixel to world coordinate 
 * conversion.
 */
class Scene {
   private:
      static const int WIDTH = 4;
      static const int HEIGHT= 3;
      ObjList objects;
      Point eye;
      void read_objects (std::ifstream &);

   public:
      Scene (char *);

      /* returns the first item intersected, given a starting point and 
       * direction point.
       */
      intersection first_visible (const Point &sp, const Point &dp) const;

      /* returns the first non-light Object intersected, given a starting point
       * and direction point.
       */
      intersection first_object (const Point &sp, const Point &dp) const;

      /* Returns, one at a time, the lights whose view is not blocked at the 
       * given location.
       */
      const Light * next_light_visible (const Point *) const;
      static Point virtual_coord (int row, int col, int h, int w);

      const Point &eye_pt () const { return eye; }

      // Returns an iterator of all lights in the scene.
      ObjList::ObjIter light_iterator (void) const {
         return objects.light_iterator();
      }
};
