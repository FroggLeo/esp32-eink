#include <cstdint>
#include <cstring>

#include "esp_err.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <pins.hpp>
#include <ssd1608.hpp>

static const char *TAG = "ssd1608";
static spi_device_handle_t epd_spi = nullptr;

esp_err_t SSD1608::init() {
    init_gpio();
    init_spi();
    epd_init();
    return ESP_OK;
}

esp_err_t SSD1608::clear() {
    return epd_clear_screen();
}

// main function
extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Eink display test");
    // initialize components
    init_gpio();
    init_spi();
    epd_init();

    if (epd_clear_screen() != ESP_OK) {
        ESP_LOGE(TAG, "Initial white refresh failed");
        while (true) {
            delay_ms(1000);
        }
    }

    clear_buffer();
    // draw stuff now!
    draw_rect(5, 5, 286, 118, false);
    draw_rect(20, 20, 20, 20);

    // refresh the display
    if (epd_refresh() != ESP_OK) {
        ESP_LOGE(TAG, "Image refresh failed");
    }

    draw_rect(19, 49, 232, 52, false);

    epd_enter_partial_mode();
    uint16_t msdelay = 5;
    while (true) {
        for (int x = 20; x < 250; x++) {
            ESP_LOGI(TAG, "Partial refresh");
            draw_rect(x, 50, 1, 50, true, true);
            if (epd_partial_refresh() != ESP_OK) {
            ESP_LOGE(TAG, "Partial refresh failed");}
            delay_ms(msdelay);
        }
        for (int x = 249; x >= 20; x--) {
            ESP_LOGI(TAG, "Partial refresh");
            draw_rect(x, 50, 1, 50, true, false);
            if (epd_partial_refresh() != ESP_OK) {
            ESP_LOGE(TAG, "Partial refresh failed");}
            delay_ms(msdelay);
        }
    }
    
}