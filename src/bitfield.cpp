#include <algorithm>
#include <cstdint>
#include "bitfield.h"
#include "utils.h"
#include "exceptions.h"

void compound_bitfield_impl::set(size_t index, value_type value)
{
    throw not_implemented();
}

auto compound_bitfield_impl::get(size_t index) const -> value_type
{
    value_type result{};
    size_t i = index * 8, end = i + 8;
    while(i < end) {
        result = (result << 1) | data[i++];
    }
    return result;
}

size_t compound_bitfield_impl::size() const
{
    return data.size() / 8;
}

size_t compound_bitfield_impl::field_count() const
{
    return fields.size();
}

void compound_bitfield_impl::prepare(generation_context &)
{
    if(data.size() % 8 != 0)
        throw invalid_field_size();
    size_t global_index = 0;
    for(const auto &f : fields) {
        size_t index = 0;
        while(index < f.size()) {
            data[global_index + index] = f[index];
            index++;
        }
        global_index += index;
    }
}

void compound_bitfield_impl::add_field(field child)
{
    data.resize(data.size() + child.size());
    fields.push_back(std::move(child));
}

auto compound_bitfield_impl::dependent_fields() const -> dependents_type
{
    dependents_type output;
    transform(
        fields.begin(),
        fields.end(),
        std::back_inserter(output),
        std::mem_fn(&field::id)
    );
    return output;
}

void compound_bitfield_impl::accept_visitor(const visitor_type& visitor) const
{
    for(const auto &f : fields) {
        f.accept_visitor(visitor);
    }
}
