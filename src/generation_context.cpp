#include "generation_context.h"

generation_context::generation_context(random_generator::result_type seed)
: rnd(seed)
{
    
}

field_mapper &generation_context::get_mapper()
{
    return mapper;
}

const field_mapper &generation_context::get_mapper() const
{
    return mapper;
}

auto generation_context::get_random_generator() -> random_generator&
{
    return rnd;
}
