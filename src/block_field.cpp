#include "block_field.h"
#include "block_value_wrapper.h"

block_field_impl::block_field_impl(size_t size) 
: v_wrapper(value_wrapper::from_impl<block_value_wrapper_impl>(size))
{
    
}

value_wrapper &block_field_impl::value()
{
    return v_wrapper;
}
