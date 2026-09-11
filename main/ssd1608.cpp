#pragma once

#include <cstddef>
#include <cstdint>

#include "esp_err.h"

namespace SSD1608 {
    constexpr int NATIVE_WIDTH = 128;
    constexpr int NATIVE_HEIGHT = 296;
    constexpr int WIDTH = 296;
    constexpr int HEIGHT = 128;
    constexpr size_t FRAMEBUFFER_SIZE = NATIVE_WIDTH * NATIVE_HEIGHT / 8;
    esp_err_t init();
    esp_err_t clear();
    esp_err_t full_refresh(const uint8_t *framebuffer);
    esp_err_t enter_partial_mode(const uint8_t *framebuffer);
    esp_err_t partial_refresh(const uint8_t *framebuffer);
}


static spi_device_handle_t epd_spi = nullptr;

// hardware default width and height
static constexpr int NATIVE_WIDTH = 128;
static constexpr int NATIVE_HEIGHT = 296;

// rotated width and height
static constexpr int WIDTH = 296;
static constexpr int HEIGHT = 128;

// define the buffer, divide by 8 for 1 bit per pixel
static constexpr int FRAMEBUFFER_SIZE = NATIVE_WIDTH * NATIVE_HEIGHT / 8;
static uint8_t framebuffer[FRAMEBUFFER_SIZE];

static void clear_buffer() {
    memset(framebuffer, 0xFF, sizeof(framebuffer)); // 0xFF for white
}
// draw a single pixel at xy coordinates
static void draw_pixel(int x, int y, bool black = true) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {return;}
    // rotate xy coordinates into panel's native coordinates
    int native_x = NATIVE_WIDTH-1-y;
    int native_y = x;
    // byte count on the screen by zigzag scanning
    int byte_index = native_y * (NATIVE_WIDTH/8) + native_x / 8;
    // creates a bitmask, you could say a bit index within the byte
    // how it works is it first creates 0x80 for int value 128 or 10000000
    // then shifts (using >>) the 1 in that byte by the specific amount calculated (native_x % 8)
    uint8_t mask = 0x80 >> (native_x % 8);
    // 0 in the buffer is black
    if (black) { 
        framebuffer[byte_index] &= ~mask; // tilde ~ flips all the bits in mask
    } else {
        framebuffer[byte_index] |= mask; 
    }
}
// draw a rectangle
static void draw_rect(int x, int y, int width, int height, bool fill = true, bool black = true) {
    if (fill) {
        for (int iy = y; iy < y + height; iy++) {
            for (int ix = x; ix < x + width; ix++) {
                draw_pixel(ix, iy, black);
            }
        }
    } else {
        for (int ix = x; ix < x + width; ix++) {
            draw_pixel(ix, y, black);
            draw_pixel(ix, y + height - 1, black);
        }
        for (int iy = y + 1; iy < y + height - 1; iy++) { // skip the corners since previous function already covered them
            draw_pixel(x, iy, black);
            draw_pixel(x + width - 1, iy, black);
        }
    }
}
// write the buffer to the e ink
static void spi_write(const uint8_t *data, size_t length)
{
    gpio_set_level(Pins::EINK_CS, 0);
    while (length > 0) {
        size_t chunk = length > 64 ? 64 : length;
        spi_transaction_t transaction = {};
        transaction.length = chunk * 8;
        transaction.tx_buffer = data;
        ESP_ERROR_CHECK(spi_device_transmit(epd_spi, &transaction));
        data += chunk;
        length -= chunk;
    }
    gpio_set_level(Pins::EINK_CS, 1);
}
// DC pin can receive commands or data
// command send helper
static void epd_command(uint8_t command) {
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_DC, 0));
    spi_write(&command, 1);
}
// data send helper
static void epd_data(const uint8_t *data, size_t length) {
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_DC, 1));
    spi_write(data, length);
}
// single byte data
static void epd_data(uint8_t data) {
    epd_data(&data, 1);
}
// initialize the spi device
static void init_spi() {
    // configure spi bus
    spi_bus_config_t bus = {};
    bus.mosi_io_num = Pins::EINK_MOSI;
    bus.miso_io_num = -1;
    bus.sclk_io_num = Pins::EINK_SCK;
    bus.quadwp_io_num = -1;
    bus.quadhd_io_num = -1;
    bus.max_transfer_sz = 64;
    // initialize the spi bus
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_DISABLED));
    // configure spi interface
    spi_device_interface_config_t device = {};
    device.clock_speed_hz = 4 * 1000 * 1000;
    device.mode = 0;
    device.spics_io_num = -1;
    device.queue_size = 1;
    // initialize spi interface
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device, &epd_spi));
}
// initialize gpio
static void init_gpio() {
    gpio_config_t outputs = {};
    outputs.pin_bit_mask =(1ULL << Pins::EINK_DC) | (1ULL << Pins::EINK_RST) | (1ULL << Pins::EINK_CS);
    outputs.mode = GPIO_MODE_OUTPUT;
    outputs.pull_up_en = GPIO_PULLUP_DISABLE;
    outputs.pull_down_en = GPIO_PULLDOWN_DISABLE;
    outputs.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&outputs));
    // start control signals in idle state
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_CS, 1));
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_DC, 1));
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_RST, 1));
    gpio_config_t busy = {};
    busy.pin_bit_mask = (1ULL << Pins::EINK_BUSY);
    busy.mode = GPIO_MODE_INPUT;
    busy.pull_up_en = GPIO_PULLUP_DISABLE;
    busy.pull_down_en = GPIO_PULLDOWN_DISABLE;
    busy.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&busy));
}
// delay helper
static void delay_ms(int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}
// reset eink
static void epd_reset() {
    gpio_set_level(Pins::EINK_RST, 1);
    delay_ms(20);
    gpio_set_level(Pins::EINK_RST, 0);
    delay_ms(20);
    gpio_set_level(Pins::EINK_RST, 1);
    delay_ms(200);
}
// delay until operation done on eink
static esp_err_t epd_wait_busy() {
    delay_ms(10);
    ESP_LOGI(TAG, "BUSY = %d", gpio_get_level(Pins::EINK_BUSY));
    for (int i = 0; i < 1000; i++) {
        if (gpio_get_level(Pins::EINK_BUSY) == 0) {
            return ESP_OK;
        }
        delay_ms(10);
    }
    // timeout
    ESP_LOGE(TAG, "Eink BUSY timeout after 10000ms");
    return ESP_ERR_TIMEOUT;
}
// lut for ssd1608
static const uint8_t full_lut[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t partial_lut[] = {
    0x10, 0x18, 0x18, 0x08, 0x18,
    0x18, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x13, 0x14, 0x44, 0x12, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00
};
// now configuring the physical e ink controller/driver
static void epd_set_full_window() {
    epd_command(0x11);
    epd_data(0x03);
    epd_command(0x44);
    epd_data(0x00);
    epd_data(0x0F);
    epd_command(0x45);
    uint8_t y_range[] = {
        0x00, 0x00,
        0x27, 0x01
    };
    epd_data(y_range, sizeof(y_range));
    epd_command(0x4E);
    epd_data(0x00);
    epd_command(0x4F);
    uint8_t y_start[] = {0x00, 0x00};
    epd_data(y_start, sizeof(y_start));
}
static void epd_enter_partial_mode() {
    epd_command(0x01);
    uint8_t gate[] = {0x27, 0x01, 0x00};
    epd_data(gate, sizeof(gate));
    epd_command(0x0C);
    uint8_t soft_start[] = {0xD7, 0xD6, 0x9D};
    epd_data(soft_start, sizeof(soft_start));
    epd_command(0x2C);
    epd_data(0xA8);
    epd_command(0x3A);
    epd_data(0x1A);
    epd_command(0x3B);
    epd_data(0x08);
    epd_set_full_window();
    epd_command(0x32);
    epd_data(partial_lut, sizeof(partial_lut));
    epd_command(0x24);
    epd_data(framebuffer, sizeof(framebuffer));
    ESP_LOGI(TAG, "Partial refresh mode ready");
}
static esp_err_t epd_full_update() {
    epd_command(0x22);
    epd_data(0xC4);
    epd_command(0x20);
    esp_err_t result = epd_wait_busy();
    epd_command(0xFF);
    return result;
}
static esp_err_t epd_partial_refresh() {
    epd_set_full_window();
    epd_command(0x24);
    epd_data(framebuffer, sizeof(framebuffer));
    epd_command(0x22);
    epd_data(0x04);
    epd_command(0x20);
    esp_err_t err = epd_wait_busy();
    epd_command(0xFF);
    if (err != ESP_OK) {return err;}
    /*
     * Important for fast differential updates:
     *
     * Write the resulting screen state back into RAM again
     * without refreshing. This establishes it as the baseline
     * for the next partial update.
     */
    epd_set_full_window();
    epd_command(0x24);
    epd_data(framebuffer, sizeof(framebuffer));
    return ESP_OK;
}
static esp_err_t epd_clear_screen() {
    epd_set_full_window();
    ESP_LOGI(TAG, "Writing white framebuffer");
    epd_command(0x24);
    uint8_t white[64];
    memset(white, 0xFF, sizeof(white));
    size_t remaining = FRAMEBUFFER_SIZE;
    while (remaining > 0) {
        size_t chunk = remaining > sizeof(white) ? sizeof(white) : remaining;
        epd_data(white, chunk);
        remaining -= chunk;
    }
    ESP_LOGI(TAG, "Refreshing white screen");
    return epd_full_update();
}
static void epd_init() {
    ESP_LOGI(TAG, "Hardware reset");
    epd_reset();
    epd_command(0x01);
    uint8_t gate[] = {0x27, 0x01, 0x00};
    epd_data(gate, sizeof(gate));
    epd_command(0x0C);
    uint8_t soft_start[] = {0xD7, 0xD6, 0x9D};
    epd_data(soft_start, sizeof(soft_start));
    epd_command(0x2C);
    epd_data(0xA8);
    epd_command(0x3A);
    epd_data(0x1A);
    epd_command(0x3B);
    epd_data(0x08);
    epd_set_full_window();
    epd_command(0x32);
    epd_data(full_lut, sizeof(full_lut));
    ESP_LOGI(TAG, "Powering panel on");
    epd_command(0x22);
    epd_data(0xC0);
    epd_command(0x20);
    ESP_ERROR_CHECK(epd_wait_busy());
    ESP_LOGI(TAG, "Panel powered on");
}
// send buffer and refresh the e ink display
static esp_err_t epd_refresh(const uint8_t *framebuffer) {
    epd_set_full_window();
    ESP_LOGI(TAG, "Sending framebuffer");
    epd_command(0x24);
    epd_data(framebuffer,
        SSD1608::FRAMEBUFFER_SIZE
    );

    ESP_LOGI(TAG, "Starting full refresh");

    esp_err_t err = epd_full_update();

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Refresh complete");
    }

    return err;
}