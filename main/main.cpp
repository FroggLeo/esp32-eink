#include "DEV_Config.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "eink/display.hpp"
#include "graphics/canvas.hpp"
#include "graphics/drawing.hpp"

#include "graphics/font.hpp"
#include "graphics/fonts/test_font.hpp"

static const char *TAG = "main";

// main function
extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Eink display test");    
    Eink display;
    display.init();
    Canvas content(128, 296, 90);
    content.buffer_fill(true);
    for (int i = 10; i < 100; i++) {
        content.draw_pixel(i, i, false);
    }
    display.refresh(content.get_buffer(), true);
    DEV_Delay_ms(1000);
    display.refresh(content.get_buffer(), true);
    DEV_Delay_ms(1000);
    display.refresh(content.get_buffer(), true);
    DEV_Delay_ms(1000);
    content.buffer_fill(true);
    draw_rect(content, 10, 10, 280, 100, true, false);
    display.refresh(content.get_buffer(), true);
    set_font(test_font);
    draw_char(content, 20, 20, 'A', true);
    draw_char(content, 30, 20, 'B', true);
    draw_char(content, 40, 20, 'C', true);
    draw_text(content, 40, 30, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", true);
    display.refresh(content.get_buffer(), false);
    return;
}