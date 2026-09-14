#pragma once

#include <stdint.h>

class Eink {
    public:
        void init();
        void clear();
        void refresh_full(uint8_t *buffer);
        void refresh_fast(uint8_t *buffer);
        void refresh_partial(uint8_t *buffer);
    private:
        enum Mode {
            FULL,
            FAST,
            PARTIAL
        };
        Mode mode;
};