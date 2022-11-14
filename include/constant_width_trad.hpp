#pragma once

#include <cstdint>

template<uint8_t width>
class const_width_trad {
    static_assert(width <= 56);
   public:
    static uint64_t get(uint64_t i, uint8_t* data) {
        if constexpr (width == 8) {
            return data[i];
        } else if constexpr (width == 16) {
            return get_be<2>(i, data);
        } else if constexpr (width == 24) {
            return get_be<3>(i, data);
        } else if constexpr (width == 32) {
            return get_be<4>(i, data);
        } else if constexpr (width == 40) {
            return get_be<5>(i, data);
        } else if constexpr (width == 48) {
            return get_be<6>(i, data);
        } else if constexpr (width == 56) {
            return get_be<7>(i, data);
        } else if constexpr (width < 8) {
            return get_by(i, data);
        } else if constexpr (width < 16) {
            return get_ua<2>(i, data);
        } else if constexpr (width < 24) {
            return get_ua<3>(i, data);
        } else if constexpr (width < 32) {
            return get_ua<4>(i, data);
        } else if constexpr (width < 40) {
            return get_ua<5>(i, data);
        } else if constexpr (width < 48) {
            return get_ua<6>(i, data);
        } else {
            return get_ua<7>(i, data);
        } 
    }

   private:
    template <uint8_t n_bytes>
    static uint64_t get_ua(uint64_t i, uint8_t* data) {
        uint64_t start = i * width;
        uint64_t start_index = start / 8;
        start %= 8;
        uint64_t res = data[start_index];
        //std::cerr << "trad fb " << start_index << ": " << std::bitset<64>(res) << std::endl;
        for (uint8_t k = 1; k < n_bytes; k++) {
            res <<= 8;
            res |= data[start_index + k];
            //std::cerr << "trad " << int(start_index + k) << ": " << std::bitset<64>(res) << std::endl;
        }
        uint8_t sw = start + width;
        if (sw <= 8 * n_bytes) {
            res >>= 8 * n_bytes - sw;
        } else {
            sw -= 8 * n_bytes;
            res = (res << sw) | (data[start_index + n_bytes] >> (8 - sw));
        }
        //std::cerr << "trad before mask and shift " << std::bitset<64>(res) << std::endl;
        return res & ((uint64_t(1) << width) - 1);
    }

    static uint8_t get_by(uint64_t i, uint8_t* data) {
        uint64_t start = i * width;
        uint64_t start_index = start / 8;
        start %= 8;
        uint8_t res = data[start_index];
        uint8_t sw = start + width;
        if (sw <= 8) {
            res >>= 8 - sw;
        } else {
            sw -= 8;
            res = (res << sw) | (data[start_index + 1] >> (8 - sw));
        }
        return res & ((uint8_t(1) << width) - 1);
    }

    template <uint8_t n_bytes>
    static uint64_t get_be(uint64_t i, uint8_t* data) {
        i *= n_bytes;
        uint64_t res = data[i];
        for (uint8_t k = 1; k < n_bytes; k++) {
            res <<= 8;
            res |= data[i + k];
        }
        return res;
    }
};