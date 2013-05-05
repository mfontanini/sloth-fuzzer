#include "generation_context.h"

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
