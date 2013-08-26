#include "Color.h"
double Color::AMB_WT = .5;

Color::Color (double r, double g, double b): 
              red(r), green(g), blue(b) {}

Color &Color::to_byte_range (void) {

   for (int i=RED; i <= BLUE; ++i) {
      (*this)[i] *= 255;
      if ((*this)[i] > 255) (*this)[i] = 255;
      if ((*this)[i] <   0) (*this)[i] =   0;
   }
   return *this;
}

Color Color::operator+ (const Color &add) const {
   return Color (red+add.red, green+add.green, blue+add.blue);
}
Color Color::operator+ (double add) const {
   return Color (red+add, green+add, blue+add);
}
Color Color::operator* (const Color &color) const {
   return Color (red *color.red, green*color.green, blue *color.blue);
}
Color Color::operator* (double mult) const {
   return Color (red*mult, green*mult, blue*mult);
}
bool Color::operator!=(const Color& c) const {
   return fabs (red-c.red) >= .001 ||
   fabs (green-c.green) >= .001 ||
   fabs (blue-c.blue) >= .001;
}
bool Color::operator==(const Color& c) const {
   return fabs (red-c.red) < .001 &&
   fabs (green-c.green) < .001 &&
   fabs (blue-c.blue) < .001;
}

const Color &Color::operator+= (const Color &color) {
   for (int i=RED; i <= BLUE; ++i) {
      (*this)[i] += color[i];
   }
   return *this;
}

const Color &Color::operator*= (const Color &color) {
   for (int i=RED; i <= BLUE; ++i) {
      (*this)[i] *= color[i];
   }
   return *this;
}

const Color &Color::operator*= (double mult) {
   for (int i=RED; i <= BLUE; ++i) {
      (*this)[i] *= mult;
   }
   return *this;
}

const Color &Color::operator/= (double divisor) {
   for (int i=RED; i <= BLUE; ++i) {
      (*this)[i] /= divisor;
   }
   return *this;
}

Color Color::complement () const {
   return Color (1.0-red, 1.0-green, 1.0-blue);
}

std::ifstream &operator>> (std::ifstream &in, Color &c) {
   in >> c.red >> c.green >> c.blue;
   return in;
}

std::ostream &operator<< (std::ostream &out, const Color &c) {
   return out << c.red << ", " << c.green << ", " << c.blue;
}

