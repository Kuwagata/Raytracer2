#include "Photon.h"

/*
See header for change log
*/

Photon::Photon (const Color &c, const Point &p): rgb(c), loc(p) {}
      
Photon::Photon (const Photon &toCopy)
{
   rgb = toCopy.rgb;
   loc = toCopy.loc;
}

Closest::Closest(Photon const *p, double d): closest(p), distance(d) {}

bool Closest::operator< (const Closest &rhs) const
{
   return distance < rhs.distance;
}

bool Closest::operator> (const Closest &rhs) const
{
   return distance > rhs.distance;
}
