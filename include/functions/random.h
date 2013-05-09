#ifndef FUZZER_FUNCTION_RANDOM_H
#define FUZZER_FUNCTION_RANDOM_H

#include "field_filler.h"

class random_function : public field_filler {
public:
    void fill(field &f, generation_context &ctx);
};

class bitrandom_function : public field_filler {
public:
    void fill(field &f, generation_context &ctx);
};

#endif // FUZZER_FUNCTION_RANDOM_H
