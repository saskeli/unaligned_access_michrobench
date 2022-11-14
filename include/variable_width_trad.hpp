#pragma once

#include <cstdint>

class variable_width_trad {
   public:
    static uint64_t get(uint64_t i, uint8_t width, uint8_t* data) {
        int32_t remaining = width;
        uint64_t idx = i / 8;
        uint64_t offs = i % 8;
        uint64_t res = data[idx++];
        remaining -= 8 - offs;
        while (remaining > 0) {
            res <<= 8;
            res |= data[idx++];
            remaining -= 8;
        }
        res >>= -remaining;
        res &= (uint64_t(1) << width) - 1;
        return res;
    }
};