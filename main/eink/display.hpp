#pragma once

#include <stdint.h>

class Eink {
    public:
        void init();
        void clear();
        void refresh(uint8_t *buffer, bool full_refresh = true);
};