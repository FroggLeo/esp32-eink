#include <cstring>

#include "DEV_Config.h"
#include "esp_log.h"

#include "eink/display.hpp"
#include "eink/Display_EPD_W21.h"
#include "graphics/canvas.hpp"

static const char *TAG = "main";

extern "C" void app_main() {
    Eink display;
    display.init();

    Canvas black(128, 296, 90);
    Canvas red(128, 296, 90);

    black.buffer_fill(true);
    red.buffer_fill(true);

    // FIRST: completely scrub whatever was previously on the panel.
    ESP_LOGI(TAG, "Normal full white refresh");
    EPD_HW_Init();
    EPD_WhiteScreen_White();
    EPD_DeepSleep();

    DEV_Delay_ms(1000);

    // THEN: enter partial-update mode from a known clean state.
    ESP_LOGI(TAG, "Setting white base map");
    EPD_HW_Init();

    EPD_SetRAMValue_BaseMap(
        black.get_buffer(),
        red.get_buffer()
    );

    // Vendor partial region:
    // PART_COLUMN = 32 pixels high
    // PART_LINE   = 64 pixels wide
    uint8_t part[32 * 64 / 8];

    // white background
    memset(part, 0xFF, sizeof(part));

    // draw a simple 32x16 black rectangle in the middle
    for (int y = 8; y < 24; y++) {
        for (int x_byte = 2; x_byte < 6; x_byte++) {
            part[y * 8 + x_byte] = 0x00;
        }
    }

    ESP_LOGI(TAG, "Partial -> black rectangle");

    EPD_Dis_Part(
        0,
        0,
        part,
        32,
        64
    );

    DEV_Delay_ms(2000);

    // erase the exact same area
    memset(part, 0xFF, sizeof(part));

    ESP_LOGI(TAG, "Partial -> white");

    EPD_Dis_Part(
        0,
        0,
        part,
        32,
        64
    );

    EPD_DeepSleep();
}
    /*
    Canvas black(128, 296, 90);
    Canvas red(128, 296, 90);
    black.buffer_fill(true);
    red.buffer_fill(true);
    set_font(test_font);
    draw_rect(black, 10, 10, 120, 50, true, false);
    draw_text(black, 30, 30, "BLACK", true);
    draw_text(black, 30, 70, "BLACK", false);
    draw_rect(red, 160, 10, 120, 50, true, false);
    draw_text(red, 180, 30, "RED", true);
    draw_text(red, 180, 70, "RED", false);
    display.refresh(black.get_buffer(), red.get_buffer());
    */
