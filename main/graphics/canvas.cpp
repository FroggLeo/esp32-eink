#include <cstdint>
#include <cstring>
#include <stdint.h>

#include "graphics/canvas.hpp"

void Canvas::draw_pixel(int x, int y, bool white) {
    int width;
    int height;
    // logical dimensions depend on rotation
    if (rotation == 0 || rotation == 180) {
        width = native_width;
        height = native_height;
    } else {
        width = native_height;
        height = native_width;
    }
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    int native_x;
    int native_y;
    switch (rotation) {
        case 0:
            native_x = x;
            native_y = y;
            break;
        case 90:
            native_x = native_width - 1 - y;
            native_y = x;
            break;
        case 180:
            native_x = native_width - 1 - x;
            native_y = native_height - 1 - y;
            break;
        case 270:
            native_x = y;
            native_y = native_height - 1 - x;
            break;
        default:
            return;
    }
    int byte_index = native_y * ((native_width + 7) / 8) + native_x / 8;
    uint8_t mask =
        0x80 >> (native_x % 8);
    // 0 in the buffer is black
    if (white) {
        framebuffer[byte_index] |= mask;
    } else {
        framebuffer[byte_index] &= ~mask;
    }
}

void Canvas::set_rotation(int new_rotation) {
    if (new_rotation == 0 || new_rotation == 90 || new_rotation == 180 || new_rotation == 270) {
        rotation = new_rotation;
        return;
    }
    rotation = 0;
}

void Canvas::buffer_fill(bool white) {
    memset(framebuffer, white ? 0xFF : 0x00, buffer_size);
}

uint8_t *Canvas::get_buffer() {
    return framebuffer;
}