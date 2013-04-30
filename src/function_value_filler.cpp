#include "function_value_filler.h"

function_value_filler::function_value_filler(unique_value value)
: value(std::move(value))
{
    
}

void function_value_filler::fill(field &f)
{
    auto data = value->eval();
    
}
