#pragma once

#include <stdint.h>

// replace Debug with ESP_LOGI
#include "esp_log.h"
#define Debug(...) ESP_LOGI("EPD", __VA_ARGS__)

// esp32 variable type compatibility
typedef uint8_t UBYTE;
typedef uint16_t UWORD;
typedef uint32_t UDOUBLE;

#ifdef __cplusplus
extern "C" {
#endif

// pins
// extern const for external constant in DEV_Config.cpp
extern const int EPD_RST_PIN;
extern const int EPD_DC_PIN;
extern const int EPD_CS_PIN;
extern const int EPD_BUSY_PIN;

// expose these functions
void DEV_Delay_ms(uint32_t ms);
void DEV_SPI_WriteByte(uint8_t value);
int DEV_Digital_Read(int pin);
void DEV_Digital_Write(int pin, int value);

#ifdef __cplusplus
}
#endif

