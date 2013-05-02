#include <algorithm>
#include "field.h"
#include "field_impl.h"
#include "endianness.h"
#include "exceptions.h"
#include "field_mapper.h"

std::atomic<field::identifier_type> field::global_id;

field::field(filler_type filler, unique_impl impl) 
: impl(std::move(impl)), filler(std::move(filler)), identifier(global_id++)
{
    
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

void field::fill(const field_mapper &mapper) 
{
    if(filler)
        filler->fill(*this, mapper);
}

void field::set_value(double value)
{
    le_insert(value);
}

double field::get_value()
{
    return le_extract();
}

double field::le_extract() 
{
    switch(size()) {
        case sizeof(uint8_t):
            return (*this)[0];
        case sizeof(uint16_t):
            return Endian::le_to_host(extract(type2type<uint16_t>()));
        case sizeof(uint32_t):
            return Endian::le_to_host(extract(type2type<uint32_t>()));
        case sizeof(uint64_t):
            return Endian::le_to_host(extract(type2type<uint64_t>()));
    }
    throw invalid_field_size();
}

uint16_t field::extract(type2type<uint16_t>) 
{
    return (*this)[0] | 
            (static_cast<uint32_t>((*this)[1]) << 8);
}

uint32_t field::extract(type2type<uint32_t>) 
{
    return (*this)[0] | 
            (static_cast<uint32_t>((*this)[1]) << 8) | 
            (static_cast<uint32_t>((*this)[2]) << 16) | 
            (static_cast<uint32_t>((*this)[3]) << 24);
}

uint64_t field::extract(type2type<uint64_t>) 
{
    return (*this)[0] |
            (static_cast<uint64_t>((*this)[1]) << 8) | 
            (static_cast<uint64_t>((*this)[2]) << 16) | 
            (static_cast<uint64_t>((*this)[3]) << 24) |
            (static_cast<uint64_t>((*this)[4]) << 32) |
            (static_cast<uint64_t>((*this)[5]) << 40) |
            (static_cast<uint64_t>((*this)[6]) << 48) |
            (static_cast<uint64_t>((*this)[7]) << 56);
}

void field::le_insert(double value)
{
    switch(size()) {
        case sizeof(uint8_t):
            (*this)[0] = static_cast<uint8_t>(value);
            break;
        case sizeof(uint16_t):
            insert(Endian::host_to_le<uint16_t>(value));
            break;
        case sizeof(uint32_t):
            insert(Endian::host_to_le<uint32_t>(value));
            break;
        case sizeof(uint64_t):
            insert(Endian::host_to_le<uint64_t>(value));
            break;
        default:
            throw invalid_field_size();
    }
}

void field::insert(uint16_t value)
{
    (*this)[0] = value & 0xff;
    (*this)[1] = (value >> 8) & 0xff;
}

void field::insert(uint32_t value)
{
    (*this)[0] = value & 0xff;
    (*this)[1] = (value >> 8) & 0xff;
    (*this)[2] = (value >> 16) & 0xff;
    (*this)[3] = (value >> 24) & 0xff;
}

void field::insert(uint64_t value)
{
    (*this)[0] = value & 0xff;
    (*this)[1] = (value >> 8) & 0xff;
    (*this)[2] = (value >> 16) & 0xff;
    (*this)[3] = (value >> 24) & 0xff;
    (*this)[4] = (value >> 32) & 0xff;
    (*this)[5] = (value >> 40) & 0xff;
    (*this)[6] = (value >> 48) & 0xff;
    (*this)[7] = (value >> 56) & 0xff;
}

void field::accept_visitor(const visitor_type &visitor) const
{
    visitor(*this);
    impl->accept_visitor(visitor);
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
