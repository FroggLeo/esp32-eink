#include <stdint.h>

#include "graphics/canvas.hpp"
#include "graphics/drawing.hpp"

void draw_rect(Canvas& canvas, int x, int y, int width, int height, bool fill = true, bool white = true) {
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