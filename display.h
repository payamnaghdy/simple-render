#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

typedef struct Display Display;

struct Display{
    uint32_t width;
    uint32_t height;
    uint16_t grid_size;
    uint16_t *buffer;
    uint16_t canvas_color;
    void (*flush_callback)(Display *disp, void *platform_data);
};


void display_init(Display *disp, uint16_t *buffer, uint16_t width, uint16_t height, void (*flush_func)(Display *, void *));
void color_canvas(Display *disp, uint16_t color);
void draw_pixel(Display *disp, uint16_t x, uint16_t y, uint16_t color);
void draw_line(Display *disp, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void draw_square(Display *disp, uint16_t x, uint16_t y, uint16_t size, bool fill, uint16_t color);
void draw_triangle(Display *disp, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void draw_grid(Display *disp, uint16_t size, uint16_t color);
void fill_grid_item(Display *disp, uint16_t x, uint16_t y, uint16_t color);
void empty_grid_item(Display *disp, uint16_t x, uint16_t y);
void draw_digit_at(Display *disp, uint8_t digit_index, uint16_t x, uint16_t y, uint16_t color, uint8_t scale);
void display_flush(Display *disp, void *platform_data);

#endif
