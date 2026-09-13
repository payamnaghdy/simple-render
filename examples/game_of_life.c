// #define LINUX
#define STM

#ifdef LINUX
#include "linux_render.h"
#endif

#ifdef STM
#include "stm32_render.h"
#endif /* ifdef STM */

void step_game(uint8_t **game, Display *disp, int m, int n) {
  uint8_t directions[8][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
  for (uint8_t i = 0; i < m; ++i) {
    for (uint8_t j = 0; j < n; ++j) {
        uint8_t alive_neighbours = 0; 
        
        for(uint8_t d=0; d<8; ++d) {
            uint8_t x = i + directions[d][0];
            uint8_t y = j + directions[d][1];
            if (x >= 0 && x < m && y >= 0 && y < n && (game[x][y] == 1 || game[x][y] == 3))
            {
              alive_neighbours++;
            }
        }
        
        bool dies = game[i][j] == 1 && ((alive_neighbours < 2) || (alive_neighbours > 3));
        if (dies) {
            game[i][j] = 3;
        }
        if (game[i][j] == 0 && alive_neighbours == 3) {
            game[i][j] = 2;
        }
    }
  }
   for (uint8_t i = 0; i < m; ++i) {
    for (uint8_t j = 0; j < n; ++j) {
        if (game[i][j] == 2) {
            game[i][j] = 1;
            fill_grid_item(disp, i, j, 0x0000);
        }
        if(game[i][j] == 3) {
            game[i][j] = 0;
            empty_grid_item(disp, i, j);
        }
      }
    }
}
int main(void) {
    uint16_t m = WIDTH / 10;
    uint16_t n = HEIGHT / 10;

    #ifdef LINUX
    uint16_t my_buffer[WIDTH * HEIGHT];

    uint8_t* game[m];
    for (uint8_t i = 0; i < m; ++i) {
        game[i] = (uint8_t*)malloc(n * sizeof(uint8_t));
    }
    #else
    static uint16_t my_buffer[WIDTH * HEIGHT];
    static uint8_t game_buffer[WIDTH / 10 * HEIGHT / 10];
    uint8_t* game[m];
    for (uint8_t i = 0; i < m; ++i) {
        game[i] = &game_buffer[i * n];
    }
    #endif

    game[10][5] = 1;
    game[10][4] = 1;
    game[10][6] = 1;

    #ifdef LINUX
    SDLRender sdl_r;
    init_sdl(&sdl_r);
    #endif 
   
    Display screen;

    #ifdef LINUX
    display_init(&screen, my_buffer, WIDTH, HEIGHT, sdl_flush_callback);
    #endif /* ifdef LINUX */
    color_canvas(&screen, 0xFFFF);
    draw_grid(&screen, 10, 0xCCC);
    
     for (uint8_t i = 0; i < m; ++i) {
      for (uint8_t j = 0; j < n; ++j) {
        if (game[i][j]) {
           fill_grid_item(&screen, i, j, 0x0000);
        }
      }
    }
    
    bool running = true;
    #ifdef LINUX
    SDL_Event event;
    #endif /* ifdef LINUX */
    
    while (running) {
        #ifdef LINUX
        while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
            running = false;
          }
        }
        #else
        RCC_APB2ENR |= (1 << 4);
        GPIOC_CRH &= ~(0xF << 20);
        GPIOC_CRH |= (0x2 << 20);
        while (1) {
        // Toggle PC13
          GPIOC_ODR ^= (1 << 13);
          delay(5000000);
        }
        #endif /* ifdef LINUX */
        step_game(game, &screen, m, n);
        #ifdef LINUX
        display_flush(&screen, &sdl_r);
        SDL_Delay(500);
        #endif /* ifdef LINUX */ 
    }
    #ifdef LINUX
    distroy_sdl(&sdl_r);
    #endif /* ifdef LINUX */
    
    return 0;
    
    }
