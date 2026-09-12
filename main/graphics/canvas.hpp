#pragma once

#include <stdint.h>

class Canvas {
    public:
        Canvas(int native_width, int native_height, int rotation) 
        : native_width(native_width), native_height(native_height), buffer_size((native_width * native_height + 7) / 8) {
            framebuffer = new uint8_t[buffer_size];
            set_rotation(rotation);
        }
        ~Canvas() {
            delete[] framebuffer;
        }
        void draw_pixel(int x, int y, bool white);
        void set_rotation(int rotation);
        void buffer_fill(bool white);
        uint8_t *get_buffer();
    private:
        int native_width;
        int native_height;
        int buffer_size;
        int rotation;
        uint8_t *framebuffer;
};