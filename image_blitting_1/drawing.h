#ifndef _DRAWING_H_
#define _DRAWING_H_

#include "color.h"

void draw_border_rect(float x, float y, float width, float height, float thickness, const color3& color);
void draw_fill_rect(float x, float y, float width, float height, const color3& color);
void draw_border_fill_rect(float x, float y, float width, float height, float thickness, const color3& fill, const color3& border);

#endif
