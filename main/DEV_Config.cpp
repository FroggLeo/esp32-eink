#pragma once

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"

#include "pins.hpp"
#include "soc/gpio_num.h"
#include "DEV_Config.h"

// pins
constexpr int EPD_RST_PIN = Pins::EINK_RST;
constexpr int EPD_DC_PIN = Pins::EINK_DC;
constexpr int EPD_CS_PIN = Pins::EINK_CS;
constexpr int EPD_BUSY_PIN = Pins::EINK_BUSY;

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

}