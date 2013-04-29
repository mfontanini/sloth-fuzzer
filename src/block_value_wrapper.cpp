#include "block_value_wrapper.h"


block_value_wrapper_impl::block_value_wrapper_impl(size_t data_size)
: data(data_size)
{
    
}

void block_value_wrapper_impl::set(size_t index, value_type value) 
{
    data[index] = value;
}

auto block_value_wrapper_impl::get(size_t index) const -> value_type
{
    return data[index];
}

size_t block_value_wrapper_impl::size() const
{
    return data.size();
}
