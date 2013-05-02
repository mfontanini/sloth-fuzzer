#include "unary_field_function.h"
#include "field_mapper.h"

unary_field_function::unary_field_function(identifier_type id)
: id(id)
{
    
}

double unary_field_function::eval(const field_mapper& mapper)
{
    return apply(mapper.find_field(id));
}

auto unary_field_function::dependent_fields() const  -> dependents_type
{
    return { id };
}

// filler

unary_field_filler_function::unary_field_filler_function(identifier_type id)
: id(id)
{
    
}

void unary_field_filler_function::fill(field &f, const field_mapper& mapper)
{
    apply(mapper.find_field(id), f);
}

auto unary_field_filler_function::dependent_fields() const -> dependents_type
{
    return { id };
}
