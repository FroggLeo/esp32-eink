#include <cstdint>

#include <stdint.h>

#include "eink/DEV_Config.h"
#include "eink/EPD_2in9.h"

class Eink {
    public:
        void init();
        void clear();
        void refresh(uint8_t *buffer, bool full_refresh);
};

void Eink::init() {
    DEV_Module_Init();
    EPD_2IN9_Init(EPD_2IN9_FULL);
}

void Eink::clear() {
    EPD_2IN9_Clear();
}

void Eink::refresh(uint8_t *buffer, bool full_refresh) {
    EPD_2IN9_Init(EPD_2IN9_FULL ? full_refresh : EPD_2IN9_PART);
    EPD_2IN9_Display(buffer);
}