#include <cstdint>
#include <cstring>

#include "esp_err.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/spi_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "hal/spi_types.h"

#include <pins.hpp>

static const char *TAG = "eink";
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
    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {return;}
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
            draw_pixel(ix, y + height, black);
        }
        for (int iy = y + 1; iy < y + height - 1; iy++) { // skip the corners since previous function already covered them
            draw_pixel(x, iy, black);
            draw_pixel(x + width, iy, black);
        }
    }
}
// write the buffer to the e ink
static void spi_write(const uint8_t *data, size_t length) {
    while (length > 0) {
        // max chunk size to 64
        size_t chunk = length > 64 ? 64 : length;
        // transaction object
        spi_transaction_t transaction = {};
        transaction.length = chunk * 8; // length in bits
        transaction.tx_buffer = data; // data from buffer
        // now send over the buffer
        ESP_ERROR_CHECK(spi_device_transmit(epd_spi, &transaction));
        data += chunk;
        length -= chunk;
    }
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
    device.spics_io_num = Pins::EINK_CS;
    device.queue_size = 1;
    // initialize spi interface
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device, &epd_spi));
}
// initialize gpio
static void init_gpio() {
    // dc and reset output
    gpio_config_t outputs = {};
    outputs.pin_bit_mask = (1ULL << Pins::EINK_DC | 1ULL << Pins::EINK_RST);
    outputs.mode = GPIO_MODE_OUTPUT;
    outputs.pull_up_en = GPIO_PULLUP_DISABLE;
    outputs.pull_down_en = GPIO_PULLDOWN_DISABLE;
    outputs.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&outputs));
    // busy input
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
}
// delay until operation done on eink
static esp_err_t epd_wait_busy() {
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
// panel update waveform data
static uint8_t full_lut[] = {
    0x50, 0xAA, 0x55, 0xAA, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0x1F, 0x00, 0x00,
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
static void epd_init() {
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
    epd_command(0x22);
    epd_data(0xC0);
    epd_command(0x20);
    ESP_ERROR_CHECK(epd_wait_busy());
}
// send buffer and refresh the e ink display
static void epd_refresh() {
    epd_set_full_window();
    ESP_LOGI(TAG, "Sending framebuffer");
    epd_command(0x24);
    epd_data(framebuffer, sizeof(framebuffer));
    ESP_LOGI(TAG, "Refreshing display");
    epd_command(0x22);
    epd_data(0xC4);
    epd_command(0x20);
    ESP_ERROR_CHECK(epd_wait_busy());
    ESP_LOGI(TAG, "Refresh complete");
}
// main function
extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Eink display test");
    // initialize components
    init_gpio();
    init_spi();
    epd_init();
    clear_buffer();
    // draw stuff now!
    draw_rect(5, 5, 286, 118, false);
    draw_rect(20, 20, 20, 20);
    // refresh the display
    epd_refresh();
}