#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"

#include "hal/spi_types.h"
#include "pins.hpp"
#include "soc/gpio_num.h"
#include "DEV_Config.h"

// pins
const int EPD_RST_PIN = Pins::EINK_RST;
const int EPD_DC_PIN = Pins::EINK_DC;
const int EPD_CS_PIN = Pins::EINK_CS;
const int EPD_BUSY_PIN = Pins::EINK_BUSY;

// device handle for spi transmit
static spi_device_handle_t epd_spi = nullptr;

extern "C" {

void DEV_Delay_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void DEV_SPI_WriteByte(uint8_t value) {
    spi_transaction_t t = {};
    t.length = 8;
    t.tx_buffer = &value;
    ESP_ERROR_CHECK(spi_device_transmit(epd_spi, &t));
}

int DEV_Digital_Read(int pin) {
    return gpio_get_level((gpio_num_t)pin);
}

void DEV_Digital_Write(int pin, int value) {
    gpio_set_level((gpio_num_t)pin, value);
}

int DEV_Module_Init() {
    // configure gpio outputs for display control
    gpio_config_t outputs = {}; 
    outputs.pin_bit_mask = (1ULL << Pins::EINK_DC) | (1ULL << Pins::EINK_RST) | (1ULL << Pins::EINK_CS);
    outputs.mode = GPIO_MODE_OUTPUT;
    outputs.pull_up_en = GPIO_PULLUP_DISABLE;
    outputs.pull_down_en = GPIO_PULLDOWN_DISABLE;
    outputs.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&outputs));
    // start control signals in idle state
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_CS, 1));
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_DC, 1));
    ESP_ERROR_CHECK(gpio_set_level(Pins::EINK_RST, 1));
    // configure gpio inputs for display status
    gpio_config_t inputs = {}; 
    inputs.pin_bit_mask = (1ULL << Pins::EINK_BUSY);
    inputs.mode = GPIO_MODE_INPUT;
    inputs.pull_up_en = GPIO_PULLUP_DISABLE;
    inputs.pull_down_en = GPIO_PULLDOWN_DISABLE;
    inputs.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&inputs));
    // configure spi bus
    spi_bus_config_t bus {};
    bus.mosi_io_num = Pins::EINK_MOSI;
    bus.miso_io_num = -1;
    bus.sclk_io_num = Pins::EINK_SCK;
    bus.quadwp_io_num = -1;
    bus.quadhd_io_num = -1;
    bus.max_transfer_sz = 64; // 64 bytes
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus, SPI_DMA_DISABLED));
    // configure spi interface
    spi_device_interface_config_t device = {};
    device.clock_speed_hz = 4 * 1000 * 1000; // 4 megahertz
    device.mode = 0;
    device.spics_io_num = -1;
    device.queue_size = 1;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device, &epd_spi));
    return 0;
}

}