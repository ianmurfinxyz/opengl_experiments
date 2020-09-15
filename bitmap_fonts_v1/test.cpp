#include <stdio.h>
#include <cmath>

#include "bitmap_fonts.h"
#include "font.h"

int main()
{
  for(const glyph& g : dogica_8px)
    print_glyph(g);
}
