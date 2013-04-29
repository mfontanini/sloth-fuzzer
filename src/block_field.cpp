#include "block_field.h"

block_field_impl::block_field_impl(size_t size) 
: data(size)
{
    
}

void block_field_impl::set(size_t index, value_type value) 
{
    data[index] = value;
}

auto block_field_impl::get(size_t index) const -> value_type
{
    return data[index];
}

size_t block_field_impl::size() const
{
    return data.size();
}
