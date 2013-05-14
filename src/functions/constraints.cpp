#include "functions/constraints.h"
#include "parser/nodes.h"
#include "exceptions.h"

required_size_constraint::required_size_constraint(size_t required)
: required(required) 
{
    
}

void required_size_constraint::check(const grammar::field_node &f)
{
    if(f.min_size() != required || f.max_size() != required)
        throw incorrect_ast_field_size(f.get_line_number(), f.to_string(), required);
}
