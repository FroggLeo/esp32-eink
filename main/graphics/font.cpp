#include <stdint.h>

#include "graphics/font.hpp"

static const Font *font = nullptr;

void set_font(const Font& new_font) {
    font = &new_font;
}

int draw_char(Canvas& canvas, int x, int y, char c, bool white) {
    if (font == nullptr) {
        return 0;
    }
    uint8_t code = static_cast<uint8_t>(c);
    // filter out characters the current font doesn't have
    if (code < font->first_char || code > font->last_char) {
        return 0;
    }
    const Glyph& glyph = font->glyphs[code - font->first_char];
    for (int row = 0; row < font->height; row++) {
        for (int col = 0; col < glyph.width; col++) {
            if (glyph.rows[row] & (0x80 >> col)) {
                canvas.draw_pixel(x + col, y + row, white);
            }
        }
    }
    return glyph.advance;
}

int draw_text(Canvas& canvas, int x, int y, const char *text, bool white) {
    int cursor_x = x;
    while (*text) {
        cursor_x += draw_char(canvas, cursor_x, y, *text, white);
        text++;
    }
    return cursor_x;
}