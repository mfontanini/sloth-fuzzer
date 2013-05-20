#include "functions/constraints.h"
#include "parser/nodes.h"
#include "exceptions.h"
#include "block_field.h"
#include "utils.h"

std::unique_ptr<field_impl> function_constraint::field_impl_from_constraint() const
{
    throw constraint_too_soft();
}


required_size_constraint::required_size_constraint(size_t required)
: required(required) 
{
    
}

void required_size_constraint::check(const grammar::field_node &f) const
{
    if(f.min_size() != required || f.max_size() != required)
        throw incorrect_ast_field_size(f.get_line_number(), f.to_string(), required);
}

std::unique_ptr<field_impl> required_size_constraint::field_impl_from_constraint() const
{
    return make_unique<block_field_impl>(required);
}
