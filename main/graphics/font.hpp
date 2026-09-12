#pragma once

#include <cstdint>
#include <stdint.h>

#include "graphics/canvas.hpp"

struct Glyph {
    uint8_t width;
    uint8_t advance;
    uint8_t rows[8];
};

struct Font {
    const Glyph *glyphs;
    uint8_t first_char;
    uint8_t last_char;
    uint8_t height;
};

void set_font(const Font& new_font);

int draw_char(Canvas& canvas, int x, int y, char c, bool white = false);
int draw_text(Canvas& canvas, int x, int y, const char *text, bool white = false);