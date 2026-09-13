#include <time.h>
#include "linux_render.h"
void delay(int sec) {
  int milli_seconds = 1000 * sec;
  clock_t start_time = clock();
  while (clock() < start_time + milli_seconds);
}
int main(void) {
    uint16_t m = WIDTH / 10;
    uint16_t n = HEIGHT / 10;


    uint16_t my_buffer[WIDTH * HEIGHT];


    SDLRender sdl_r;
    init_sdl(&sdl_r);

   
    Display screen;

    display_init(&screen, my_buffer, WIDTH, HEIGHT, sdl_flush_callback);

    color_canvas(&screen, 0xFFFF);
 
    bool running = true;
    SDL_Event event;
    uint8_t i = 0; 
    while (running) {
        while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
            running = false;
          }
        }
        display_flush(&screen, &sdl_r);
        color_canvas(&screen, 0xFFFF);
        draw_digit_at(&screen, i, 10, 10, 0x0000, 1);
        draw_digit_at(&screen, i, 30, 10, 0x0000, 2);
        draw_digit_at(&screen, 8, 50, 10, 0x0000, 4);
        SDL_Delay(500);
        i = (i + 1) % 10;
        delay(1);
        // color_canvas(&screen, 0xFFFF);
    }
    distroy_sdl(&sdl_r);    
    return 0;
    
    }
