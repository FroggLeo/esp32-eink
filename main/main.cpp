#include "DEV_Config.h"
#include "esp_log.h"

#include "eink/display.hpp"
#include "graphics/canvas.hpp"
#include "graphics/drawing.hpp"
#include "graphics/font.hpp"
#include "graphics/fonts/test_font.hpp"

static const char *TAG = "main";

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting display test");

    Eink display;
    display.init();

    Canvas black(128, 296, 90);
    set_font(test_font);

    draw_text(black, 10, 10, "FULL");
    draw_sample(black, 10, 20);
    display.refresh_full(black.get_buffer());

    draw_text(black, 10, 40, "FAST");
    draw_sample(black, 10, 50);
    display.refresh_fast(black.get_buffer());

    draw_text(black, 10, 70, "PARTIAL");
    draw_sample(black, 10, 80);
    display.refresh_partial(black.get_buffer());
}
