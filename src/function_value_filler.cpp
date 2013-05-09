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

void function_value_filler::fill(field &f, generation_context &ctx)
{
    f.set_value(value->eval(ctx));
}
