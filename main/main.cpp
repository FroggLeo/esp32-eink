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

    draw_sample(black, 10, 10);
    draw_sample(red, 10, 25);

    display.refresh(black.get_buffer(), red.get_buffer());

    DEV_Delay_ms(2000);
    draw_rect(black, 5, 40, 253, 1, true, false);
    draw_sample(black, 10, 45);
    
    display.fast_bw_refresh(black.get_buffer(), red.get_buffer());
    draw_sample(black, 10, 60);
    display.fast_bw_refresh(black.get_buffer(), red.get_buffer());
}
