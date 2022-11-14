#pragma once

#include <cstdint>
#include <endian.h>

class variable_width_wider {
   public:
    static uint64_t get(uint64_t i, uint8_t width, uint8_t* data) {
        uint64_t idx = i / 8;
        uint64_t offs = i % 8;
        uint64_t res = be64toh(reinterpret_cast<uint64_t*>(data + idx)[0]);
        res >>= 64 - offs - width;
        res &= (uint64_t(1) << width) - 1;
        return res;
    }
};