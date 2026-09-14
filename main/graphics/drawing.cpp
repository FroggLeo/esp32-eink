#include <cstdint>
#include <stdint.h>

#include "graphics/canvas.hpp"
#include "graphics/drawing.hpp"
#include "graphics/font.hpp"
#include "graphics/fonts/test_font.hpp"

void draw_rect(Canvas& canvas, int x, int y, int width, int height, bool fill, bool white) {
    if (fill) {
        for (int iy = y; iy < y + height; iy++) {
            for (int ix = x; ix < x + width; ix++) {
                canvas.draw_pixel(ix, iy, white);
            }
        }
    } else {
        for (int ix = x; ix < x + width; ix++) {
            canvas.draw_pixel(ix, y, white);
            canvas.draw_pixel(ix, y + height - 1, white);
        }
        for (int iy = y + 1; iy < y + height - 1; iy++) { // skip the corners since previous function already covered them
            canvas.draw_pixel(x, iy, white);
            canvas.draw_pixel(x + width - 1, iy, white);
        }
    }
}

void draw_sample(Canvas& canvas, int x, int y, bool white) {
    set_font(test_font);
    draw_text(canvas, x+1, y+1, "HELLO", false);
    draw_rect(canvas, x, y+9, 31, 1, true, white);
    draw_rect(canvas, x+33, y, 10, 10, true, white);
    draw_rect(canvas, x+45, y, 10, 10, false, white);
    draw_rect(canvas, x+33, y, 10, 10, true, white);
    draw_text(canvas, x+89, y+1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", white);
    for (int i = 0; i <= 154; i+=2) {
        canvas.draw_pixel(x+i+89, y+9, white);
    }
    for (int i = 0; i < 10; i++) {
        canvas.draw_pixel(x+57+i, y+i, white);
        canvas.draw_pixel(x+58+i, y+i, white);
        canvas.draw_pixel(x+59+i, y+i, white);
        canvas.draw_pixel(x+61+i, y+i, white);
        canvas.draw_pixel(x+62+i, y+i, white);
        canvas.draw_pixel(x+64+i, y+i, white);
        canvas.draw_pixel(x+65+i, y+i, white);
        canvas.draw_pixel(x+67+i, y+i, white);
        canvas.draw_pixel(x+69+i, y+i, white);
        canvas.draw_pixel(x+71+i, y+i, white);
        canvas.draw_pixel(x+74+i, y+i, white);
        canvas.draw_pixel(x+77+i, y+i, white);
    }
}