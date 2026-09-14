#include <cstdint>

#include "eink/display.hpp"

#include "eink/DEV_Config.h"
#include "eink/EPD_2in9_V2.h"

void Eink::init() {
    DEV_Module_Init();
    EPD_2IN9_V2_Init();
    mode = Mode::FULL;
}

void Eink::clear() {
    if (mode != Mode::FULL) {
        EPD_2IN9_V2_Init();
        mode = Mode::FULL;
    }
    EPD_2IN9_V2_Clear();
}

void Eink::refresh_full(uint8_t *buffer) {
    if (mode != Mode::FULL) {
        EPD_2IN9_V2_Init();
        mode = Mode::FULL;
    }
    EPD_2IN9_V2_Display_Base(buffer);
}

void Eink::refresh_fast(uint8_t *buffer) {
    if (mode != Mode::FAST) {
        EPD_2IN9_V2_Init_Fast();
        mode = Mode::FAST;
    }
    EPD_2IN9_V2_Display_Base(buffer);
}

void Eink::refresh_partial(uint8_t *buffer) {
    mode = Mode::PARTIAL;
    EPD_2IN9_V2_Display_Partial(buffer);
}