#include "DEV_Config.h"
#include "esp_log.h"

#include "eink/display.hpp"
#include "graphics/canvas.hpp"
#include "graphics/drawing.hpp"
#include "graphics/font.hpp"
#include "graphics/fonts/test_font.hpp"

static const char *TAG = "main";

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting SSD1680A-style fast B/W LUT test");

    Eink display;
    display.init();

    Canvas black(128, 296, 90);
    Canvas red(128, 296, 90);

    black.buffer_fill(true);
    red.buffer_fill(true);
    set_font(test_font);

    draw_text(black, 10, 12, "HELLO", false);
    draw_rect(black, 43, 10, 10, 10, true, false);
    draw_rect(black, 57, 10, 10, 10, false, false);
    draw_text(black, 100, 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", false);
    draw_text(red, 10, 27, "HELLO", false);
    draw_rect(red, 43, 25, 10, 10, true, false);
    draw_rect(red, 57, 25, 10, 10, false, false);
    draw_text(red, 100, 27, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", false);
    for (int i = 0; i <= 10; i++) {
        black.draw_pixel(70+i, 10+i, false);
        black.draw_pixel(71+i, 10+i, false);
        black.draw_pixel(73+i, 10+i, false);
        black.draw_pixel(74+i, 10+i, false);
        black.draw_pixel(76+i, 10+i, false);
        black.draw_pixel(78+i, 10+i, false);
        black.draw_pixel(81+i, 10+i, false);
        black.draw_pixel(84+i, 10+i, false);
        red.draw_pixel(70+i, 25+i, false);
        red.draw_pixel(71+i, 25+i, false);
        red.draw_pixel(73+i, 25+i, false);
        red.draw_pixel(74+i, 25+i, false);
        red.draw_pixel(76+i, 25+i, false);
        red.draw_pixel(78+i, 25+i, false);
        red.draw_pixel(81+i, 25+i, false);
        red.draw_pixel(84+i, 25+i, false);
    }

    display.refresh(black.get_buffer(), red.get_buffer());

    DEV_Delay_ms(2000);
    draw_rect(black, 5, 40, 255, 1, true, false);
    draw_text(black, 10, 47, "HELLO", false);
    draw_rect(black, 43, 45, 10, 10, true, false);
    draw_rect(black, 57, 45, 10, 10, false, false);
    draw_text(black, 100, 47, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", false);
    for (int i = 0; i <= 10; i++) {
        black.draw_pixel(70+i, 45+i, false);
        black.draw_pixel(71+i, 45+i, false);
        black.draw_pixel(73+i, 45+i, false);
        black.draw_pixel(74+i, 45+i, false);
        black.draw_pixel(76+i, 45+i, false);
        black.draw_pixel(78+i, 45+i, false);
        black.draw_pixel(81+i, 45+i, false);
        black.draw_pixel(84+i, 45+i, false);
    }
    display.fast_bw_refresh(black.get_buffer(), red.get_buffer());
    draw_text(black, 10, 62, "HELLO", false);
    draw_rect(black, 43, 60, 10, 10, true, false);
    draw_rect(black, 57, 60, 10, 10, false, false);
    draw_text(black, 100, 62, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", false);
    for (int i = 0; i <= 10; i++) {
        black.draw_pixel(70+i, 60+i, false);
        black.draw_pixel(71+i, 60+i, false);
        black.draw_pixel(73+i, 60+i, false);
        black.draw_pixel(74+i, 60+i, false);
        black.draw_pixel(76+i, 60+i, false);
        black.draw_pixel(78+i, 60+i, false);
        black.draw_pixel(81+i, 60+i, false);
        black.draw_pixel(84+i, 60+i, false);
    }
    display.fast_bw_refresh(black.get_buffer(), red.get_buffer());
}
