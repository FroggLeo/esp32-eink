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

void Eink::fast_bw_refresh(uint8_t *black_buffer, uint8_t *red_buffer) {
    // Two 125 Hz passes gives much better development on thin and isolated
    // black pixels than a single pass on this panel. Red is rewritten into
    // RAM on each pass but its LUT is disabled, so existing red is preserved.
    EPD_HW_Init_Fast_BW_A();
    EPD_WhiteScreen_BW_Fast_A(black_buffer, red_buffer);

    EPD_HW_Init_Fast_BW_A();
    EPD_WhiteScreen_BW_Fast_A(black_buffer, red_buffer);
}
