#pragma once

#include <cstdint>
#include <endian.h>

template<uint8_t width>
class const_width_wider {
    static_assert(width <= 56);
   public:
    static uint64_t get(uint64_t i, uint8_t* data) {
        if constexpr (width == 8) {
            return data[i];
        } else if constexpr (width == 16) {
            return be16toh(reinterpret_cast<uint16_t*>(data + i * 2)[0]);
        } else if constexpr (width == 32) {
            return be32toh(reinterpret_cast<uint32_t*>(data + i * 4)[0]);
        } else if constexpr (width < 8) {
            return get_by(i, data);
        } else if constexpr (width <= 24) {
            return get_9_24(i, data);
        } else {
            return get_25_56(i, data);
        }
    }

   private:
    static uint32_t get_9_24(uint64_t i, uint8_t* data) {
        uint64_t start = width * i;
        uint64_t start_index = start / 8;
        start %= 8;
        uint32_t res = be32toh(reinterpret_cast<uint32_t*>(data + start_index)[0]);
        //std::cerr << "wider before mask and rsift " << std::bitset<32>(res) << std::endl;
        res >>= 32 - start - width;
        //std::cerr << "wider before mask " << std::bitset<32>(res) << std::endl;
        res &= (uint32_t(1) << width) - 1;
        //std::cerr << "wider " << std::bitset<32>(res) << std::endl;
        return res;
    }

    static uint64_t get_25_56(uint64_t i, uint8_t* data) {
        uint64_t start = width * i;
        uint64_t start_index = start / 8;
        start %= 8;
        uint64_t res = be64toh(reinterpret_cast<uint64_t*>(data + start_index)[0]);
        return (res >> (64 - start - width)) & ((uint64_t(1) << width) - 1);
    }

    static uint16_t get_by(uint64_t i, uint8_t* data) {
        uint64_t start = width * i;
        uint64_t start_index = start / 8;
        start %= 8;
        uint16_t res = be16toh(reinterpret_cast<uint16_t*>(data + start_index)[0]);
        return (res >> (16 - start - width)) & ((uint16_t(1) << width) - 1);
    }
};