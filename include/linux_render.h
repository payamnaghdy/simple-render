#ifndef LINUX_H
#define LINUX_H
#include <SDL2/SDL.h>
#include "display.h"

#define WIDTH 296
#define HEIGHT 128

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
} SDLRender;


void init_sdl(SDLRender *sdl_r);
void distroy_sdl(SDLRender *sdl_r);
void sdl_flush_callback(Display *disp, void *platform_data);

#endif
