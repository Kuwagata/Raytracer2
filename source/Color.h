#ifndef COLOR_CLASS
#define COLOR_CLASS
#include <fstream>
#include <math.h>

/* this enumerated type defined RED as 0, GREEN as 1, and BLUE as 2. */
typedef enum {RED, GREEN, BLUE} channel;

class Color {
   private:
      /* this union defines red, green, and blue in the same
       * memory space as rgb[0], rgb[1], and rgb[2]
       */
      union {
         struct {
            double red, green, blue;
         };
         double rgb[3];
      };
   public:
      /* AMB_WT specifies what percentage of diffuse lighting ambient light
       * should be set to.
       */
      static double AMB_WT;

      /* This constructor accepts 0, 1, 2, or 3 doubles and will use default 
       * values for parameters not provided.
       */
      Color (double r=0.0, double g=0.0, double b=0.0);
      double get_red(void)  const { return red; }
      double get_green(void)const { return green; }
      double get_blue(void) const { return blue; }
      void set_red(double r)  { red=r; }
      void set_green(double g){ green=g; }
      void set_blue(double b) { blue=b; }

      /* These methods provide access to the channels using indexes.
       * e.g. color[RED] or color[0]
       */
      double &operator[] (int i) { return rgb[i];}
      double  operator[] (int i) const { return rgb[i];}
      Color &to_byte_range (void);
      const Color &operator+= (const Color &);
      Color operator+ (const Color &add) const;
      Color operator+ (double add) const;
      Color operator* (const Color &color) const;
      Color operator* (double mult) const;
      bool operator!=(const Color& c) const;
      bool operator==(const Color& c) const;
      const Color &operator/= (double);
      const Color &operator*= (const Color &);
      const Color &operator*= (double);

      /* returns a constant black color for convenience */
      static const Color &black() {
         static Color BLACK;
         return BLACK;
      }

      Color complement () const;

      /* Input operator allowing Color objects to be read in from an
       * input stream using the input operator (>>)
       */
      friend std::ifstream &operator>> (std::ifstream &in, Color &c);

      /* Output operator defining how a Color object is printed
       * when used with the output operator (<<).
       */
      friend std::ostream &operator<< (std::ostream &out, const Color &c);
};
#endif
