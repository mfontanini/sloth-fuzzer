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

#ifndef FUZZER_BLOCK_FIELD_H
#define FUZZER_BLOCK_FIELD_H

#include <vector>
#include "field_impl.h"
#include "exceptions.h"

template<typename Container>
class generic_block_field_impl : public clonable_field_impl<generic_block_field_impl<Container>> {
public:
    typedef Container container_type;
    typedef field_impl::buffer_iterator buffer_iterator;
    using field_impl::get_value;

    generic_block_field_impl(size_t data_size)
    : data(data_size)
    {
        
    }

    void set(size_t index, field_impl::value_type value)
    {
        data[index] = value;
    }
    
    field_impl::value_type get(size_t index) const
    {
        return data[index];
    }
    
    size_t size() const
    {
        return data.size();
    }
    
    void set_value(int64_t value) 
    {
        field_impl::set_value(value);
    }
    
    int64_t get_value() const
    {
        return field_impl::get_value();
    }

    buffer_iterator fill_from_buffer(buffer_iterator start, buffer_iterator end)
    {
        if(std::distance(start, end) < static_cast<ptrdiff_t>(data.size()))
            throw not_enough_data();
        std::copy(start, start + data.size(), data.begin());
        return start + data.size();
    }
private:
    container_type data;
};

typedef generic_block_field_impl<std::vector<field_impl::value_type>> block_field_impl;

#endif // FUZZER_BLOCK_FIELD_H
