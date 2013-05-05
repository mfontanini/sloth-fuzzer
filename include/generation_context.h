#ifndef FUZZER_GENERATION_CONTEXT_H
#define FUZZER_GENERATION_CONTEXT_H

#include <random>
#include "field_mapper.h"

class generation_context {
public:
    typedef std::random_device random_generator;

    field_mapper &get_mapper();
    const field_mapper &get_mapper() const;
    random_generator &get_random_generator();
private:
    field_mapper mapper;
    random_generator rnd;
};

#endif // FUZZER_GENERATION_CONTEXT_H
