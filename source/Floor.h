#include <math.h>
#include "Object.h"

/* The class defines an infinite, checkered plane below the eyepoint */
class Floor : public Object {
   private:
      double height;
      Color diffuse1, ambient1, diffuse2, ambient2;

   public:
      Floor (double, const Color &, const Color &, const Color &);
      /* This constructor allows the Floor object to be read from an
       * input stream.
       */
      Floor (std::ifstream &in);
      virtual Color get_ambient (const Point &) const;
      virtual Color get_diffuse (const Point &spot) const { 
         int floorX = (int) floor (spot[X]);
         int floorZ = (int) floor (spot[Z]);

         return ((floorX+floorZ)%2==0)? diffuse1 : diffuse2;
      }
      virtual bool get_intersect(const Point &sp, const Point &dp,
                         intersection &intersect);
      virtual Point get_normal (const Point &) const {
         static Point normal (0, 1, 0);
         return normal;
      }

      /* This output operator allows the floor object to be printed */
      friend std::ostream& operator<< (std::ostream &out, const Floor &) {
         return out << "Floor";
      }
};
