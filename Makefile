CFLAGS = -std=c++2a -Wall -Wextra -Wshadow -pedantic -march=native

HEADERS = include/constant_width_trad.hpp include/constant_width_wider.hpp

.PHONY: clean

.DEFAULT: all

%/%.hpp:

all: gen_bytes constant_width_bench variable_width_bench

gen_bytes: gen_bytes.cpp
	g++ $(CFLAGS) -DNDEBUG -Ofast -o gen_bytes gen_bytes.cpp

constant_width_bench: constant_width_bench.cpp $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast -o constant_width_bench constant_width_bench.cpp

variable_width_bench: variable_width_bench.cpp $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast -o variable_width_bench variable_width_bench.cpp

test: test.cpp $(HEADERS)
	g++ $(CFLAGS) -DNDEBUG -Ofast -o test test.cpp

clean:
	rm -f make_bwt
	rm -f constant_width_bench