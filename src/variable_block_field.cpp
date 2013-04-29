#include "variable_block_field.h"

variable_block_field_impl::variable_block_field_impl(size_t min_size, size_t max_size) 
: distribution(min_size, max_size)
{
    
}

void variable_block_field_impl::set(size_t index, value_type value) 
{
    data[index] = value;
}

auto variable_block_field_impl::get(size_t index) const -> value_type
{
    return data[index];
}

size_t variable_block_field_impl::size() const
{
    return data.size();
}

void variable_block_field_impl::prepare(random_generator &engine) {
    data.resize(distribution(engine));
}
