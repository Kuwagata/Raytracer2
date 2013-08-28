#include "Color.h"
#include "Point.h"
#include <stdlib.h>

/*
Changes, as per Dr. Matzko's provided code:
   -Added comparison operators for closest photons
   -Made all variables public to facilitate access
   -Simplified constructors from previous version
   -Combined the ClosestPhoton class into the Photon class
*/

class Photon
{      
   public:
      Color rgb;
      Point loc;
      
      Photon () {}
      Photon (const Color &c, const Point &p);
      Photon (const Photon &toCopy);
};

class Closest
{
   public:
      Photon const * closest;
      double distance;
      
      Closest(Photon const *p, double d);
      Closest() { closest = NULL; }
      bool operator< (const Closest &rhs) const;
      bool operator> (const Closest &rhs) const;
};
