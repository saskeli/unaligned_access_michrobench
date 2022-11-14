#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "include/constant_width_trad.hpp"
#include "include/constant_width_wider.hpp"

typedef std::chrono::duration<double, std::nano> d_nano;

template <uint8_t width>
std::pair<double, double> bench(uint64_t n, uint64_t count, uint8_t* data, std::vector<uint64_t>& locs) {
    
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;

    uint64_t a_c = 0;
    for (uint64_t i = 0; i < count; i++) {
        a_c += const_width_wider<width>::get(locs[i], data);
    }

    uint64_t b_c = 0;
    double trad_time = 0;
    for (uint64_t i = 0; i < count; i++) {
        auto t_a = high_resolution_clock::now();
        b_c += const_width_trad<width>::get(locs[i], data);
        auto t_b = high_resolution_clock::now();
        trad_time += duration_cast<d_nano>(t_b - t_a).count();
    }

    uint64_t c_c = 0;
    double wide_time = 0;
    for (uint64_t i = 0; i < count; i++) {
        auto t_a = high_resolution_clock::now();
        c_c += const_width_wider<width>::get(locs[i], data);
        auto t_b = high_resolution_clock::now();
        wide_time += duration_cast<d_nano>(t_b - t_a).count();
    }

    std::cerr << int(width) << "\t" << trad_time / count << "\t" << wide_time / count << "\t";
    if ((a_c == b_c) && (b_c == c_c)) {
        std::cerr << "OK";
    } else {
        std::cerr << (b_c > c_c ? b_c - c_c : c_c - b_c);
    }
    std::cerr << std::endl;
    if constexpr (width > 1) {
        auto r = bench<width - 1>(n, count, data, locs);
        trad_time += r.first;
        wide_time += r.second;
    }
    return {trad_time, wide_time};
}

int main(int argc, char const* argv[]) {
    if (argc < 4) {
        std::cerr << "Read n bytes from file and run i operations\n\n"
                  << "Usage:\n"
                  << "  ./constant_width_branch n i in_file" << std::endl;
    }
    uint64_t n, i;
    std::string in_path(argv[3]);
    std::sscanf(argv[1], "%lu", &n);
    std::sscanf(argv[2], "%lu", &i);

    uint8_t* data = (uint8_t*)malloc(n);

    std::fstream in_file;
    in_file.open(in_path, std::ios::binary | std::ios::in);
    in_file.read(reinterpret_cast<char*>(data), n);

    uint64_t seed = 1337;
    std::mt19937 mt(seed);
    std::uniform_int_distribution<unsigned long long> gen(0, n / 9 - 1);
    std::vector<uint64_t> locs;

    for (uint64_t j = 0; j < i; j++) {
        locs.push_back(gen(mt));
    }
    std::cerr << "width\ttrad\twide\tcheck\n";
    auto p = bench<56>(n, i, data, locs);
    std::cerr << "w / t = " << p.second / p.first << std::endl;

    return 0;
}
