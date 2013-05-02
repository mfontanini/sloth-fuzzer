#include "function_value_filler.h"
#include "field.h"

function_value_filler::function_value_filler(unique_value value)
: value(std::move(value))
{
    
}

auto function_value_filler::dependent_fields() const -> dependents_type
{
    return value->dependent_fields();
}

void function_value_filler::fill(field &f, const field_mapper &mapper)
{
    f.set_value(value->eval(mapper));
}
