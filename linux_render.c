#include "linux_render.h"

void init_sdl(SDLRender *sdl_r) {
    SDL_Init(SDL_INIT_VIDEO);
    sdl_r->window = SDL_CreateWindow("Renderer", 100, 100, WIDTH, HEIGHT, 0);
    sdl_r->renderer = SDL_CreateRenderer(sdl_r->window, -1, 0);
    sdl_r->texture = SDL_CreateTexture(sdl_r->renderer, SDL_PIXELFORMAT_RGB565, 
                                SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
}
void distroy_sdl(SDLRender *sdl_r) {
    SDL_DestroyRenderer(sdl_r->renderer);
    SDL_DestroyWindow(sdl_r->window);
    SDL_Quit();
}

void sdl_flush_callback(Display *disp, void *platform_data) {
    SDLRender * sdl_r = (SDLRender *)platform_data;
    SDL_UpdateTexture(sdl_r->texture, NULL, disp->buffer, disp->width * sizeof(uint16_t));
    SDL_RenderClear(sdl_r->renderer);
    SDL_RenderCopy(sdl_r->renderer, sdl_r->texture, NULL, NULL);
    SDL_RenderPresent(sdl_r->renderer);
}
