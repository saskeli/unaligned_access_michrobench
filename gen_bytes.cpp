#include <iostream>
#include <random>

static const uint16_t BUFFER_SIZE = 1000;

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        std::cerr << "Number required\n"
                  << "Will output at least n bytes of random data" << std::endl;
        exit(1);
    }
    uint64_t n;
    std::sscanf(argv[1], "%lu", &n);
    uint64_t i = 0;

    std::independent_bits_engine<std::default_random_engine, 8, uint8_t> bytes;
    uint8_t buffer[BUFFER_SIZE];
    while (i < n) {
        for (uint64_t j = 0; j < BUFFER_SIZE; j++) {
            buffer[j] = bytes();
        }
        std::fwrite(buffer, 1, BUFFER_SIZE, stdout);
        i += BUFFER_SIZE;
    }
    std::fflush(stdout);
    return 0;
}
