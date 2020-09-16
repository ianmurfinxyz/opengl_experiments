
#include <SDL2/SDL_opengl.h>
#include <string>
#include "color.h"
#include "font.h"

//
// note - the functions in this module will not restore the opengl state to what it was upon
// returning, thus each function must setup the opengl server to the desire state prior to
// issuing the draw request.
//

//
// Client side vertex buffer used by the rect drawing functions. Vertices are ordered 
// as illustrated below:
//
//     [6,7]       [4,5]
//       x-----------x
//       |           |    where the numbers in brackets are the indices into the
//       |           |    'draw_rect' array.
//       x-----------x
//     [0,1]       [2,3]
//
static float draw_rect[8];

void draw_border_rect(float x, float y, float width, float height, float thickness, const color3& color)
{
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);

  // will use vertex arrays to render the rect and assume that all arrays are deactivate upon
  // invocation, thus enable the vertex array on the server.
  glEnableClientState(GL_VERTEX_ARRAY);

  // compute the vertices of the rect.
  draw_rect[0] = x;
  draw_rect[1] = y;
  draw_rect[2] = x + width;
  draw_rect[3] = y;
  draw_rect[4] = draw_rect[2];
  draw_rect[5] = y + height;
  draw_rect[6] = x;
  draw_rect[7] = draw_rect[5];

  // inform the server where to find the vertex data.
  glVertexPointer(2, GL_FLOAT, 0, draw_rect);

  // set the active draw color on the server.
  glColor3f(color.r, color.g, color.b);

  // set the drawing mode such that we draw an unfilled border box.
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // set the correct line thickness.
  glLineWidth(thickness);

  // issue the draw call to the server.
  glDrawArrays(GL_POLYGON, 0, 4);
}

void draw_fill_rect(float x, float y, float width, float height, const color3& color)
{
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  draw_rect[0] = x;
  draw_rect[1] = y;
  draw_rect[2] = x + width;
  draw_rect[3] = y;
  draw_rect[4] = draw_rect[2];
  draw_rect[5] = y + height;
  draw_rect[6] = x;
  draw_rect[7] = draw_rect[5];

  glVertexPointer(2, GL_FLOAT, 0, draw_rect);
  glColor3f(color.r, color.g, color.b);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawArrays(GL_POLYGON, 0, 4);
}

void draw_border_fill_rect(float x, float y, float width, float height, float thickness, const color3& fill, const color3& border)
{
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  draw_rect[0] = x;
  draw_rect[1] = y;
  draw_rect[2] = x + width;
  draw_rect[3] = y;
  draw_rect[4] = draw_rect[2];
  draw_rect[5] = y + height;
  draw_rect[6] = x;
  draw_rect[7] = draw_rect[5];

  glVertexPointer(2, GL_FLOAT, 0, draw_rect);

  glColor3f(fill.r, fill.g, fill.b);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawArrays(GL_POLYGON, 0, 4);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(border.r, border.g, border.b);
  glLineWidth(thickness);
  glDrawArrays(GL_POLYGON, 0, 4);
}

void draw_text(float x, float y, const std::string &text, const bitmap_font &font, const color3 &font_color)
{
  glColor3f(font_color.r, font_color.g, font_color.b);
  glRasterPos2f(x, y);
  for(char c : text) {
    int index = static_cast<int>(c & 0xff) - 33;

    if(index == -1){
      glBitmap(0, 0, 0, 0, font.word_spacing_px, 0, nullptr);
      continue;
    }

    if(!(0 <= index && index < ascii_glyph_count))
      continue;

    const glyph &g = font.glyphs[index];
    glBitmap(g.width_px, g.height_px, g.bearing_x_px, g.decender_px, g.advance_px + font.glyph_spacing_px, 0, g.bitmap);
  }
  glFlush();
}
