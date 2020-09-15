#include <iostream>
#include "font.h"

void print_glyph(const glyph& g)
{
  std::cout << std::dec;
  std::cout << "=====================================" << std::endl;
  std::cout << "character: " << g.ascii_code << std::endl;
  std::cout << "ascii_code: " << static_cast<unsigned int>(g.ascii_code & 0xFF) << std::endl;
  std::cout << "bearing_x: " << static_cast<int>(g.bearing_x_px & 0xFF) << "px" << std::endl;
  std::cout << "decender: " << static_cast<int>(g.decender_px & 0xFF) << "px" << std::endl;
  std::cout << "advance: " << static_cast<int>(g.advance_px & 0xFF) << "px" << std::endl;
  std::cout << "width: " << static_cast<int>(g.width_px & 0xFF) << "px" << std::endl;
  std::cout << "height: " << static_cast<int>(g.height_px & 0xFF) << "px" << std::endl;
  std::cout << "bitmap: {" << std::endl;
  std::cout << std::hex;
  for(const char& c : g.bitmap)
    std::cout << static_cast<int>(c & 0xFF) << std::endl;
  std::cout << "}" << std::endl;
}

