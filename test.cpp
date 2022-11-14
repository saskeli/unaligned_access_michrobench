#include <fstream>
#include <iostream>
#include <bitset>

#include "include/constant_width_trad.hpp"
#include "include/constant_width_wider.hpp"

int main(int argc, char const* argv[]) {
    if (argc < 3) {
        std::cerr << "Read n bytes from file and test\n\n"
                  << "Usage:\n"
                  << "  ./constant_width_branch n in_file" << std::endl;
    }
    uint64_t n;
    std::string in_path(argv[2]);
    std::sscanf(argv[1], "%lu", &n);

    uint8_t* data = (uint8_t*)malloc(n);

    std::fstream in_file;
    in_file.open(in_path, std::ios::binary | std::ios::in);
    in_file.read(reinterpret_cast<char*>(data), n);

    uint64_t first = 0;
    uint64_t rows = 2;
    for (uint64_t i = 0; i < rows; i++) {
        std::cout << std::bitset<8>(data[first]) << " " << std::bitset<8>(data[first + 1]) << std::endl;    
        first += 2;
    }

    uint64_t a = const_width_trad<17>::get(0, data);
    uint64_t b = const_width_wider<17>::get(0, data);
    std::cout << 8 << ": " << a << " <-> " << b << std::endl;
    if (a != b) {
        std::cout << std::bitset<64>(a) << "\n"
                  << std::bitset<64>(b) << std::endl;
    }//*/

    /*for (uint64_t i = 0; i < 16; i++) {
        uint64_t a = const_width_trad<17>::get(i, data);
        uint64_t b = const_width_wider<17>::get(i, data);
        std::cout << i << ": " << a << " <-> " << b << std::endl;
        if (a != b) {
            std::cout << std::bitset<64>(a) << "\n"
                      << std::bitset<64>(b) << std::endl;
        }
    }//*/

    return 0;
}
