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

#ifndef FUZZER_BITFIELD_H
#define FUZZER_BITFIELD_H

#include <vector>
#include <tuple>
#include "field_impl.h"
#include "field.h"
#include "block_field.h"
#include "exceptions.h"

class compound_bitfield_impl : public clonable_field_impl<compound_bitfield_impl> {
public:
    void set(size_t index, value_type value);
    value_type get(size_t index) const;
    size_t size() const;
    size_t field_count() const;
    void prepare(generation_context &);
    
    void add_field(field child);
    dependents_type dependent_fields() const;
    void accept_visitor(const visitor_type& visitor) const;
private:
    typedef std::vector<field> fields_type;

    fields_type fields;
    std::vector<bool> data;
};

typedef generic_block_field_impl<std::vector<bool>> bitfield_impl;

template<>
inline void generic_block_field_impl<std::vector<bool>>::set_value(int64_t value)
{
    for(auto iter = data.rbegin(); iter != data.rend(); ++iter) {
        *iter = value & 1;
        value >>= 1;
    }
    if(value > 0)
        throw value_too_large();
}

template<>
inline int64_t generic_block_field_impl<std::vector<bool>>::get_value() const
{
    uint64_t value{};
    for(auto iter = data.rbegin(); iter != data.rend(); ++iter) {
        value = (value << 1) | static_cast<bool>(*iter);
    }
    return value;
}

#endif
