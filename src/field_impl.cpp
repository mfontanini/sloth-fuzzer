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

#include <cstdint>
#include "field_impl.h"
#include "exceptions.h"

void field_impl::set_value(int64_t value)
{
    int index = size() - 1;
    while(index >= 0 && value != 0) {
        set(index--, static_cast<uint8_t>(value) & 0xff);
        value >>= 8;
    }
}

int64_t field_impl::get_value() const
{
    int64_t val{};
    int end = size() - 1, index = end;
    while(index >= 0 && static_cast<size_t>(end - index) < sizeof(int64_t)) {
        val = val | (static_cast<uint64_t>(get(index)) << ((end - index) * 8));
        index--;
    }
    return val;
}

size_t field_impl::field_count() const 
{ 
    return 1; 
}

void field_impl::prepare(generation_context &) 
{

}

auto field_impl::dependent_fields() const -> dependents_type 
{ 
    return { };
};

auto field_impl::fill_from_buffer(buffer_iterator start, buffer_iterator end) -> buffer_iterator
{
    throw field_cant_be_used_in_response();
}