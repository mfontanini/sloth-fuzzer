#include <algorithm>
#include "field.h"
#include "value_wrapper.h"
#include "field_impl.h"


field::field(unique_impl impl) 
: impl(std::move(impl))
{
    
}

field::field(const field &rhs) 
: impl(rhs.impl ? rhs.impl->clone() : nullptr)
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

value_wrapper &field::value() {
    return impl->value();
}

const value_wrapper &field::value() const {
    return const_cast<field&>(*this).value();
}

void swap(field &lhs, field &rhs) 
{
    using std::swap;
    swap(lhs.impl, rhs.impl);
}
