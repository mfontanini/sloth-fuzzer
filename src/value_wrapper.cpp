#include "value_wrapper.h"

value_wrapper::value_wrapper(std::unique_ptr<value_wrapper_impl> impl) 
: m_impl(std::move(impl))
{
    
}

value_wrapper::value_wrapper(const value_wrapper &rhs)
: m_impl(rhs.m_impl ? rhs.m_impl->clone() : nullptr)
{
    
}

value_wrapper::value_wrapper(value_wrapper &&rhs)
{
    swap(*this, rhs);
}

value_wrapper& value_wrapper::operator=(value_wrapper rhs) 
{
    swap(*this, rhs);
    return *this;
}

size_t value_wrapper::size() const 
{
    return m_impl->size();
}

value_wrapper_impl &value_wrapper::impl() 
{
    return *m_impl;
}

const value_wrapper_impl &value_wrapper::impl() const 
{
    return *m_impl;
}

auto value_wrapper::begin() -> iterator
{
    return *m_impl;
}

auto value_wrapper::begin() const -> const_iterator
{
    return *m_impl;
}

auto value_wrapper::end() -> iterator
{
    return { *m_impl, m_impl->size() };
}

auto value_wrapper::end() const -> const_iterator
{
    return { *m_impl, m_impl->size() };
}



// other stuff

void swap(value_wrapper &lhs, value_wrapper &rhs)
{
    using std::swap;
    swap(lhs.m_impl, rhs.m_impl);
}
