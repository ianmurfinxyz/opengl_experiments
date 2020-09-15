#ifndef _FONT_H_
#define _FONT_H_

#include <iostream>
#include <cmath>

using U8 = unsigned char;
using I8 = char;

constexpr U8 operator"" _bin(const char *binary, long unsigned int n)
{
  U8 sum {0};
  for(int i = n - 1; i >= 0; --i) 
    sum += (binary[i] == '1') ? pow(2, (n - 1) - i) : 0;
  return sum;
}

constexpr I8 operator"" _px(unsigned long long int px){return px;}

//
// glyph for a bitmap font.
//
//       bearing_x
//        |====|
//        |     00000 0                    =
//        |    0     00                    |
//        |    0      0                    |
//        |    0     00                    |
// origin o-----00000-0-=--->  baseline    | height
//        |           0 |                  |
//        |           0 | decender         |
//        |     000000  =                  =
//             |======|
//              width
//
struct glyph
{
  U8 ascii_code;
  I8 bearing_x_px;
  I8 decender_px;
  I8 advance_px;
  I8 width_px;
  I8 height_px;
  U8 bitmap[8];
};

void print_glyph(const glyph& g);

//class bitmap_font
//{
//public:
//
//
//private:
//  glyph glyphs[94]; // 94 printable ascii characters
//  U8 leading_px;
//  U8 glyph_spacing_px;
//};

#endif
