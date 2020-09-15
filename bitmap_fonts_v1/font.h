
using U8 = unsigned char;
using F32 = float;

constexpr U8 operator"" _hex(const char *bin_str)
{

}

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
  U8 bearing_x_px;
  U8 decender_px;
  U8 advance_px;
  U8 width_px;
  U8 height_px;
  U8 bitmap[8];
};

class bitmap_font
{
public:


private:
  glyph glyphs[94]; // 94 printable ascii characters
  U8 leading_px;
  U8 glyph_spacing_px;
};


