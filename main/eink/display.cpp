#include <cstdint>

#include "eink/display.hpp"

#include "eink/DEV_Config.h"
#include "eink/Display_EPD_W21.h"

void Eink::init() {
    DEV_Module_Init();
}

void Eink::clear() {
    EPD_HW_Init();
    EPD_WhiteScreen_White();
    EPD_DeepSleep();
}

void Eink::refresh(uint8_t *black_buffer, uint8_t *red_buffer, bool fast_refresh) {
    if (fast_refresh) {
        EPD_HW_Init_Fast();
        EPD_WhiteScreen_ALL_Fast(black_buffer, red_buffer);
    } else {
        EPD_HW_Init();
        EPD_WhiteScreen_ALL(black_buffer, red_buffer);
    }
    EPD_DeepSleep();
}