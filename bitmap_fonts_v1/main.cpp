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
#include "bitmap_fonts.h"

#define SCREEN_WIDTH_PX 1500
#define SCREEN_HEIGHT_PX 800

static SDL_GLContext glcontext;
static SDL_Window *window;

static bool is_done {false};

void draw_scene_1();
void draw_scene_2();

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

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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

    else if(event.key.keysym.sym == SDLK_2)
      draw_scene_2();
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

  SDL_GL_SwapWindow(window);
}

void draw_scene_2()
{
  // a nice sunset orange.
  glClearColor(255_rgb, 227_rgb, 189_rgb, 255_rgb);
  glClear(GL_COLOR_BUFFER_BIT);

  color3 deep_navy {67_rgb, 25_rgb, 247_rgb};
  color3 pink {255_rgb, 79_rgb, 149_rgb};

  draw_border_fill_rect(200.f, 300.f, 100.f, 100.f, 2.f, deep_navy, pink);

  std::string text1{"abcdefghijklmnopqrstuvwxyz<>:;=?@|~[]\\/^_`'\"!#$%&()*+,-.0123456789ABCDEFGHIJKLMNOPQRSTVWXYZhellokingsandcountrymen"};
  std::string text2{"this is the end of the world as we know it and I feel fine!"};

  draw_text(500.f, 300.f, text1, dogica_8px, deep_navy);
  draw_text(500.f, 330.f, text2, dogica_8px, deep_navy);

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

