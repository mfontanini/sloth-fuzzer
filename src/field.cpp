#include <algorithm>
#include "field.h"
#include "field_impl.h"


field::field(filler_type filler, unique_impl impl) 
: impl(std::move(impl)), filler(std::move(filler))
{
    
}

field::field(const field &rhs) 
: impl(rhs.impl ? rhs.impl->clone() : nullptr), filler(rhs.filler)
{
    
}

field::field(field &&rhs) noexcept 
{
    swap(*this, rhs);
}
    
field& field::operator=(field rhs) 
{
    swap(*this, rhs);
    return *this;
}

size_t field::size() const {
    return impl->size();
}

auto field::begin() -> iterator
{
    return *impl;
}

auto field::begin() const -> const_iterator
{
    return *impl;
}

auto field::end() -> iterator
{
    return { *impl, impl->size() };
}

auto field::end() const -> const_iterator
{
    return { *impl, impl->size() };
}

auto field::operator[](size_t index) -> dereference_helper<value_type>
{
    return *(begin() + index);
}

auto field::operator[](size_t index) const -> dereference_helper<const value_type>
{
    return *(begin() + index);
}

void field::prepare(random_generator &engine)
{
    impl->prepare(engine);
}

void field::fill() 
{
    if(filler)
        filler->fill(*this);
}

void swap(field &lhs, field &rhs) 
{
    using std::swap;
    swap(lhs.impl, rhs.impl);
}
