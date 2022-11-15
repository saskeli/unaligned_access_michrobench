#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "include/variable_width_trad.hpp"
#include "include/variable_width_wider.hpp"

typedef std::chrono::duration<double, std::nano> d_nano;

int main(int argc, char const* argv[]) {
    if (argc < 4) {
        std::cerr << "Read n bytes from file and run i operations\n\n"
                  << "Usage:\n"
                  << "  ./constant_width_branch n i in_file" << std::endl;
        exit(1);
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
    std::uniform_int_distribution<uint64_t> i_gen(0, (n - 9) * 8);
    std::uniform_int_distribution<uint8_t> w_gen(1, 56);
    std::vector<std::pair<uint64_t, uint8_t>> locs;

    std::cerr << "trad\twide\tcheck\n";

    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;

    uint64_t a_c = 0;
    for (uint64_t j = 0; j < i; j++) {
        locs.push_back({i_gen(mt), w_gen(mt)});
        a_c += variable_width_wider::get(locs[j].first, locs[j].second, data);
    }    

    uint64_t b_c = 0;
    double trad_time = 0;
    for (uint64_t j = 0; j < i; j++) {
        auto t_a = high_resolution_clock::now();
        b_c += variable_width_trad::get(locs[j].first, locs[j].second, data);
        auto t_b = high_resolution_clock::now();
        trad_time += duration_cast<d_nano>(t_b - t_a).count();
    }

    uint64_t c_c = 0;
    double wide_time = 0;
    for (uint64_t j = 0; j < i; j++) {
        auto t_a = high_resolution_clock::now();
        c_c += variable_width_wider::get(locs[j].first, locs[j].second, data);
        auto t_b = high_resolution_clock::now();
        wide_time += duration_cast<d_nano>(t_b - t_a).count();
    }

    std::cerr << trad_time / i << "\t" << wide_time / i << "\t";
    if ((a_c == b_c) && (b_c == c_c)) {
        std::cerr << "OK";
    } else {
        std::cerr << (b_c > c_c ? b_c - c_c : c_c - b_c);
    }
    std::cerr << std::endl;
    std::cerr << "w / t = " << wide_time / trad_time << std::endl;

    return 0;
}
