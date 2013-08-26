#ifndef POINT_CLASS
#define POINT_CLASS
#include <math.h>
#include <assert.h>
#include <fstream>
#include <iostream>

/* this enumerated type specifies X as 0, Y as 1, and Z as 2. */
typedef enum {X, Y, Z} coord;

/* this macro multiplies the passed-in value by itself. */
#define SQUARE(x)((x)*(x))

bool equals (double a, double b);

class Point {
   private:
      /* this union stores x, y, and z in the same space as xyz[0], xyz[1],
       * and xyz[2] */
      union {
         struct {
            double x, y, z;
         };
         double xyz[3];
      };
   public:
      /* MAX distance allowed in the scene. */
      static const int MAX = 60;

      /* DIST_WT is the weight the distance from the light carries */
      static double DIST_WT;

      /* MIN distance recognized as not the same location */
      static double MIN;
      static double refractindex;
      static double air_refractindex;

      Point (double x_in=0.0, double y_in=0.0, double z_in=0.0);
      double get_x(void) const { return x; }
      double get_y(void) const { return y; }
      double get_z(void) const { return z; }
      void set_x(double x) { this->x = x; }
      void set_y(double y) { this->y = y; }
      void set_z(double z) { this->z = z; }
      /* these operators allow access to components by index. e.g. 
       * point[X] or point[0].
       */
      double &operator[] (int i) { return xyz[i];}
      double operator[] (int i) const { return xyz[i];}
      bool   operator!= (const Point &pt) const;
      double dot_prod (const Point &pt) const;
      Point operator- (const Point &pt) const;
      Point operator- (void) const {
         return Point (-x, -y, -z);
      }
      Point &operator-= (double sub) {
         x -= sub;
         y -= sub;
         z -= sub;
         return *this;
      }

      Point operator/ (double divisor) const;
      Point &operator/= (double divisor) {
         x /= divisor;
         y /= divisor;
         z /= divisor;
         return *this;
      }
      Point operator* (double mult) const;
      friend Point operator- (double mult, const Point &pt) {
         return Point (mult-pt.x, mult-pt.y, mult-pt.z);
      }
      friend Point operator* (double mult, const Point &pt) {
         return Point (mult*pt.x, mult*pt.y, mult*pt.z);
      }

      const Point &operator-= (const Point &pt);
      Point operator+ (const Point &pt) const {
         return Point (x+pt.x, y+pt.y, z+pt.z);
      }
      const Point &operator+= (const Point &pt);
      double distance (const Point &pt) const;
      Point get_unit_vector (const Point &dp) const;
      Point &normalize (void);
      Point bounce (const Point &normal) const;
      Point bend (const Point &normal, double index) const;
      bool operator==(const Point& pt) const {
         return equals (x, pt.x) && equals (y, pt.y) && equals (z, pt.z);
      }
      static Point &zero () {
         static Point pt(0.0, 0.0, 0.0);
         return pt;
      }
      friend std::ostream &operator<< (std::ostream &out, const Point &pt) {
         return out << pt.x << ", " << pt.y << ", " << pt.z;
      }

      friend std::ifstream &operator>> (std::ifstream &in, Point &pt) {
         in >> pt.x >> pt.y >> pt.z;
         return in;
      }
};
#endif
