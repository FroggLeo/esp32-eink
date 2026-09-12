#include <cstdint>
#include <cstring>

#include "esp_err.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <pins.hpp>

static const char *TAG = "ssd1608";

// main function
extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Eink display test");    
    
}