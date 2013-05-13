#include <algorithm>
#include "field.h"
#include "field_impl.h"
#include "endianness.h"
#include "exceptions.h"
#include "field_mapper.h"
#include "generation_context.h"

std::atomic<field::identifier_type> field::global_id(1);

field::identifier_type field::generate_id()
{
    return global_id++;
}

field::field(filler_type filler, unique_impl impl) 
: impl(std::move(impl)), filler(std::move(filler)), identifier(global_id++)
{
    
}

field::field(identifier_type id, filler_type filler, unique_impl impl)
: impl(std::move(impl)), filler(std::move(filler)), identifier(id)
{
    if(identifier == invalid_id)
        identifier = global_id++;
}

field::field(const field &rhs) 
: impl(rhs.impl ? rhs.impl->clone() : nullptr), filler(rhs.filler), 
identifier(rhs.identifier)
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

size_t field::size() const 
{
    return impl->size();
}

size_t field::field_count() const
{
    return impl->field_count();
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

void field::prepare(generation_context &context)
{
    impl->prepare(context);
}

void field::fill(generation_context &context) 
{
    if(filler) 
        filler->fill(*this, context);
}

void field::set_value(int64_t value)
{
    if(value == 0.0)
        std::fill(begin(), end(), 0);
    else
        impl->set_value(value);
}

int64_t field::get_value() const
{
    return impl->get_value();
}

void field::accept_visitor(const visitor_type &visitor) const
{
    visitor(*this);
    impl->accept_visitor(visitor);
}

auto field::dependent_fields() const -> dependents_type
{
    auto fields = impl->dependent_fields();
    if(filler) {
        auto filler_fields = filler->dependent_fields();
        fields.insert(fields.end(), filler_fields.begin(), filler_fields.end());
    }
    return fields;
}

bool operator<(const field &lhs, const field &rhs)
{
    return lhs.id() < rhs.id();
}

void swap(field &lhs, field &rhs) 
{
    using std::swap;
    swap(lhs.impl, rhs.impl);
    swap(lhs.filler, rhs.filler);
    swap(lhs.identifier, rhs.identifier);
}
