#include "display.h"
#include  "utils.h"
#include "font.h"

void display_init(Display *disp, uint16_t *buffer, uint16_t width, uint16_t height, void (*flush_func)(Display *, void *)) {
    disp->buffer = buffer;
    disp->width = width;
    disp->height = height;
    disp->grid_size = 0;
    disp->canvas_color = 0;
    disp->flush_callback = flush_func;
}

void color_canvas(Display *disp, uint16_t color) {
  disp->canvas_color = color;
  for (uint16_t x = 0; x < disp->width; ++x) {
    for(uint16_t y = 0; y < disp->height; ++y) {
      disp->buffer[y*disp->width + x] = color;     
    }
  }
}

void draw_pixel(Display *disp, uint16_t x, uint16_t y, uint16_t color) {
    if (x >= 0 && x < disp->width && y >= 0 && y < disp->height) {
        disp->buffer[y * disp->width + x] = color;
    }
}

void draw_line_h(Display *disp, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, float dx, float dy, uint16_t color) {
    int16_t step = 1;

    if (dy < 0) {
        dy = -1 * dy;
        step = -1;
    }

    float p = 2 * dy - dx;
    uint16_t x = x1;
    uint16_t y = y1;

    while (x != x2) {
      draw_pixel(disp, x, y, color);
      x += step;
      p  = p + 2 * dy - (p >= 0) * 2 * dx;
      y = y + (p >= 0) * step;
    }
    
}

void draw_line_v(Display *disp, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, float dx, float dy, uint16_t color) {
    int16_t step = 1;

    if (dx < 0) {
        dx = -1 * dx;
        step = -1;
    }

    float p = 2 * dx - dy;
    int x = x1;
    int y = y1;

    while (y != y2) {
      draw_pixel(disp, x, y,  color);
      y += step;
      p = p + 2 * dx - (p >= 0) * 2 * dy;
      x = x + (p >= 0) * step;
    }
}

void draw_line(Display *disp, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) { 
    float dx = x2 - x1;
    float dy = y2 - y1;
    float m;
    if (dx < 0.01) {
      //inf but does not matter its larger than 1
      m = 10.0;
    } else {
      m = dy/dx;
    }
    
    if (float_abs(m) < 1.0) {
        draw_line_h(disp, x1, y1, x2, y2, dx, dy, color);
    } else {
        draw_line_v(disp, x1, y1, x2, y2, dx, dy, color);
    }

 }
void draw_triangle(Display *disp, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    draw_line(disp, x0, y0, x1, y1, color);
    draw_line(disp, x0, y0, x2, y2, color);
    draw_line(disp, x1, y1, x2, y2, color);
}
void draw_square(Display *disp, uint16_t x, uint16_t y, uint16_t size ,bool fill, uint16_t color) {
  draw_line(disp, x, y, x + size, y, color);
  draw_line(disp, x, y, x, y + size, color);
  draw_line(disp,x + size, y, x + size, y + size, color);
  draw_line(disp, x, y + size, x + size, y + size, color);
  
  if (!fill) {
    return;
  }

  for(uint16_t i = x; i < x + size; ++i) {
    for(uint16_t j = y; j < y + x; ++j) {
       disp->buffer[j*disp->width + i] = color;  
    }
  }
}

void fill_grid_item(Display *disp, uint16_t x, uint16_t y, uint16_t color) {
  x = x * disp->grid_size;
  y = y * disp->grid_size;
  
  for(uint16_t i = x +  1; i < x + disp->grid_size; ++i) {
    for(uint16_t j = y + 1; j < y + disp->grid_size; ++j) {
       disp->buffer[j*disp->width + i] = color;  
    }
  }
}

void empty_grid_item(Display *disp, uint16_t x, uint16_t y) {
  x = x * disp->grid_size;
  y = y * disp->grid_size;

  for(uint16_t i = x + 1; i < x + disp->grid_size; ++i) {
    for(uint16_t j = y + 1; j < y + disp->grid_size; ++j) {
       disp->buffer[j*disp->width + i] = disp->canvas_color;  
    }
  }
}
void draw_grid(Display *disp, uint16_t size, uint16_t color) {
  disp->grid_size = size;
  for (uint16_t x = 0; x < disp->width; x += size) {
    for(uint16_t y = 0; y < disp->height; y += size) {
      draw_line(disp, x, y, x + size, y, color);
      draw_line(disp, x, y, x, y + size, color); 
    }
  }  
}

void draw_digit_at(Display *disp, uint8_t digit_index, uint16_t x, uint16_t y, uint16_t color, uint8_t scale) {
    for (uint16_t j = 0; j < 8; ++j) {
        uint8_t row = persian_digits[digit_index][j];
        for (uint16_t i = 0; i <= 8; ++i) {
          for (uint8_t sy = 0; sy < scale; sy++) {
              for (uint8_t sx = 0; sx < scale; sx++) {

              uint16_t target_x = x + (i*scale) + sx;
              uint16_t target_y = y + (j*scale) + sy;

              if (target_x >= 0 && target_x < disp->width && target_y >= 0 && target_y < disp->height) {
                  if (row & (1 << (7 - i))) {
                    draw_pixel(disp, target_x, target_y, color);
                  }
              }
          }
        }
        }
    }
}

void display_flush(Display *disp, void *platform_data) {
    if (disp->flush_callback) {
        disp->flush_callback(disp, platform_data);
    }
}
