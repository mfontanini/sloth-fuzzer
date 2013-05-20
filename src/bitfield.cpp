/*
 * Copyright (c) 2012, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

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

auto compound_bitfield_impl::fill_from_buffer(buffer_iterator start, buffer_iterator end) 
-> buffer_iterator
{
    size_t index{}, total = size();
    if(std::distance(start, end) < static_cast<ptrdiff_t>(total))
        throw not_enough_data();
    while(index < total) {
        set(index++, *start++);
    }
    return start;
}