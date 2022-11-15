# unaligned_access_michrobench

Testing 2 different options for accessing non-byte-aligned elements in arrays, both where element width is constant and variable

## Traditional approach

Build the result by reading bytes and bitshifting.

The start location of the `i`<sup>th</sup> character is at `i * width / 8`, this is read along with following bytes untill enough is read. Then the result is shifted to the correct position and masked to remove leading "garbage"

If element width is varaiable, `i` will directly be the starting location instead of logical index. (first byte to be read is simply `i / 8`.)

## Alternative

Read sufficiently many bytes directly and just do the bitshifts and masking. E.g. if the width of the element is 11 bits, reading 32 bits from the `i * width / 8`<sup>th</sup> byte is guaranteed to contain all that is needed. Modern systems support fairly fast big-endian loads, so all the necessary data can bea read in the correct byte-order with

```c++
uint32_t res = be32toh(reinterpret_cast<uint32_t*>(data + i * width / 8)[0]);
```

which compiles to a single `movbe` instruction.

# Usage

Clone and compile:

```bash
git clone git@github.com:saskeli/unaligned_access_michrobench.git
cd unaligned_access_michrobench
make
```

Generate squence of random bytes for testing:

```bash
./gen_bytes 9000000 > d.bin
```

Benchmark and compare for constant width access:

```bash
./constant_width_bench 9000000 10000000 d.bin
```

Benchmark and compare for variable width access:

```bash
./variable_width_bench 9000000 100000000 d.bin
```

# Results

The traditional approach "should" be slower especially in the variable width case, as it generates just under 1 branch missprediction per query.

However, the actual performance differences on the 2 test systems is minimal. Probably data dependence in the pipeline aloing with the `movbe` instruction not being quite as efficient as alternatives in terms of uops slow down the alternative approach somewhat.

Also on Intel consumer CPUs, the alternative approach may be a bit slower still since `movbe` takes ~3 cycles there as opposed to the one cycle on Intel Xenon and AMD ZENx.