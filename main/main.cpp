#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "eink/display.hpp"
#include "graphics/canvas.hpp"

static const char *TAG = "main";

// main function
extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Eink display test");    
    Eink display;
    display.init();
    Canvas content(128, 296, 270);
    content.buffer_fill(false);
    display.refresh(content.get_buffer(), true);
}