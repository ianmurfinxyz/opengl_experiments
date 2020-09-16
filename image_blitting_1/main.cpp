//
// Experiment 1: Basic Geometry
//
// The point of this test is to gain greater understanding of the content of chapter 1 of the 
// opengl 2.1 red book which deals with geometry assembly and buffer objects.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string>
#include "drawing.h"
#include "color.h"

#define SCREEN_WIDTH_PX 1500
#define SCREEN_HEIGHT_PX 800

static SDL_GLContext glcontext;
static SDL_Window *window;

//
// from opengl 2.1 red book
//
const int check_img_w_px {64};
const int check_img_h_px {64};
GLubyte check_img[check_img_w_px][check_img_h_px][3];
void make_check_img()
{
  for(int i = 0; i < check_img_h_px; ++i) {
    for(int j = 0; j < check_img_w_px; ++j) {
      int c = ( ((i & 0x8) == 0) ^ ((j & 0x08) == 0) ) * 255;
      check_img[i][j][0] = static_cast<GLubyte>(c);
      check_img[i][j][1] = static_cast<GLubyte>(c);
      check_img[i][j][2] = static_cast<GLubyte>(c);
    }
  }
}
// END


#define PINK_F32_RGB {1.f, 0.f, 1.f}
#define BLUE_F32_RGB {0.f, 0.6f, 1.f}
const int alien_w_px {8};
const int alien_h_px {8};
float alien_img_F32_RGB[alien_w_px][alien_h_px][3] = {
  {PINK_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB},
  {BLUE_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB},
  {BLUE_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, BLUE_F32_RGB},
  {PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB},
  {PINK_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, PINK_F32_RGB},
  {BLUE_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB},
  {BLUE_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, BLUE_F32_RGB},
  {BLUE_F32_RGB, BLUE_F32_RGB, BLUE_F32_RGB, PINK_F32_RGB, PINK_F32_RGB, BLUE_F32_RGB, BLUE_F32_RGB, BLUE_F32_RGB},
};

static bool is_done {false};

void draw_scene_1();

constexpr float operator"" _rgb(unsigned long long int c)
{
  if(c < 0)
    c = 0;
  else if(c > 255)
    c = 255;

  return static_cast<float>(c) / 255.f;
}

void init()
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "fatal: failed to init SDL2: SDL error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow("sdl2-opengl", 
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH_PX,
                            SCREEN_HEIGHT_PX,
                            SDL_WINDOW_OPENGL);
  if(!window) {
    fprintf(stderr, "fatal: failed to create window: SDL error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  glcontext = SDL_GL_CreateContext(window);
  if(!glcontext) {
    fprintf(stderr, "fatal: failed to create opengl context: SDL error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  //
  // These tests will all be rendering 2D geometry directly to the window thus it is desired to
  // set the current matrices of the projection stack and the modelview stack to an orthographic
  // projection matrix and the identity matrix respectively. All scenes will use these stack
  // configurations.
  //
  
  // set active matrix stack to opengl projection stack.
  glMatrixMode(GL_PROJECTION); 

  // set current matrix on active stack to the identity matrix.
  glLoadIdentity();            

  // SDL can provide the actual drawable size (pixels) of the active window. This is preferable to
  // relying on the defined SCREEN_WIDTH_PX/SCREEN_HEIGHT_PX values as it accounts for high DPI
  // display. These values are required to set the projection matrix and the viewport.
  // see: https://wiki.libsdl.org/SDL_GL_GetDrawableSize?highlight=%28%5CbCategoryVideo%5Cb%29%7C%28CategoryEnum%29%7C%28CategoryStruct%29
  int w, h;
  SDL_GL_GetDrawableSize(window, &w, &h);

  // multiply the current matrix by the orthographic projection matrix generated from the args.
  // see: https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml
  glOrtho(static_cast<GLdouble>(0), 
          static_cast<GLdouble>(w), 
          static_cast<GLdouble>(0), 
          static_cast<GLdouble>(h),
          static_cast<GLdouble>(-1), 
          static_cast<GLdouble>(1));

  // set active matrix stack to the model-view stack and set the current matrix to the identity
  // matrix.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // The viewport defines the region of the window in which to map the output of the rendering
  // pipeline. To avoid distortion of the mapped image it is neccessary to ensure the aspect ratio 
  // of the viewport matches that of the clipping planes of the view frustrum/box.
  glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));


  make_check_img();
}

static void shutdown()
{
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void on_event(SDL_Event& event)
{
  switch(event.type)
  {
  case SDL_QUIT:
    is_done = true;
    break;

  case SDL_WINDOWEVENT:
    switch(event.window.event)
    {
    case SDL_WINDOWEVENT_RESIZED:
      glViewport(0, 0, static_cast<GLsizei>(event.window.data1), static_cast<GLsizei>(event.window.data2));
      break;
    }
    break;

  case SDL_KEYDOWN:
    if(event.key.repeat)
      break;

    if(event.key.keysym.sym == SDLK_1)
      draw_scene_1();
  }
}

void draw_scene_1()
{
  // a nice sky blue.
  glClearColor(204_rgb, 254_rgb, 255_rgb, 255_rgb);
  glClear(GL_COLOR_BUFFER_BIT);

  color3 orange {255_rgb, 205_rgb, 75_rgb};
  color3 pink {255_rgb, 79_rgb, 149_rgb};
  color3 purple {135_rgb, 79_rgb, 255_rgb};

  draw_border_rect(20.f, 20.f, 300.f, 150.f, 5.f, orange);
  draw_fill_rect(200.f, 200.f, 400.f, 250.f, pink);
  draw_border_fill_rect(100.f, 600.f, 500.f, 100.f, 3.f, purple, orange);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glRasterPos2f(400.f, 400.f);
  glDrawPixels(check_img_w_px, check_img_h_px, GL_RGB, GL_UNSIGNED_BYTE, check_img);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glRasterPos2f(700.f, 400.f);
  glPixelZoom(6.f, 6.f);
  glDrawPixels(alien_w_px, alien_h_px, GL_RGB, GL_FLOAT, alien_img_F32_RGB);
  glPixelZoom(1.f, 1.f);

  SDL_GL_SwapWindow(window);
}

static void run()
{
  SDL_Event event;
  while(!is_done) {
    while(SDL_PollEvent(&event))
      on_event(event);

    SDL_Delay(2);
  } 
}

int main(int argc, char *argv[])
{
  init();
  draw_scene_1();
  run();
  shutdown();
}

