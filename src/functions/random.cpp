#include <random>
#include <algorithm>
#include <cstdint>
#include "functions/random.h"
#include "field.h"
#include "generation_context.h"

void random_function::fill(field &f, generation_context &ctx)
{
    std::uniform_int_distribution<uint8_t> generator(0, 0xff);
    std::generate(
        f.begin(), 
        f.end(), 
        [&]{ return generator(ctx.get_random_generator()); }
    );
}

void bitrandom_function::fill(field &f, generation_context &ctx)
{
    std::uniform_int_distribution<uint8_t> generator(0, 1);
    std::generate(
        f.begin(), 
        f.end(), 
        [&]{ return generator(ctx.get_random_generator()); }
    );
}
