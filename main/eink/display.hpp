#pragma once

#include <stdint.h>

class Eink {
    public:
        void init();
        void clear();
        void refresh(uint8_t *black_buffer, uint8_t *red_buffer, bool fast_refresh = false);
};