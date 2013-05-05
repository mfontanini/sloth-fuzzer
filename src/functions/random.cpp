#include <random>
#include <algorithm>
#include <cstdint>
#include "functions/random.h"
#include "field.h"

void random_function::fill(field &f, const field_mapper& mapper)
{
    std::random_device rnd;
    std::uniform_int_distribution<uint8_t> generator(0, 0xff);
    std::generate(
        f.begin(), 
        f.end(), 
        [&]{ return generator(rnd); }
    );
}
