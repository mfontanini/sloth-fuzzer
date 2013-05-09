#include <algorithm>
#include "const_value_node.h"
#include "field.h"
#include "exceptions.h"


const_value_node::const_value_node(float value)
: value(value)
{
    
}

double const_value_node::eval(generation_context &) {
    return value;
}


const_string_node::const_string_node(std::string value)
: value(std::move(value))
{
    
}

void const_string_node::fill(field &f, generation_context &) 
{
    if(f.size() < value.size())
        throw invalid_field_size();
    std::copy(value.begin(), value.end(), f.begin());
}
