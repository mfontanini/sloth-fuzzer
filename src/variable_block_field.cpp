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

#include "variable_block_field.h"
#include "generation_context.h"
#include "exceptions.h"

variable_block_field_impl::variable_block_field_impl(size_t min_size, size_t max_size) 
: distribution(min_size, max_size)
{
    
}

void variable_block_field_impl::set(size_t index, value_type value) 
{
    data[index] = value;
}

auto variable_block_field_impl::get(size_t index) const -> value_type
{
    return data[index];
}

size_t variable_block_field_impl::size() const
{
    return data.size();
}

void variable_block_field_impl::prepare(generation_context &context) 
{
    data.resize(distribution(context.get_random_generator()));
}

auto variable_block_field_impl::fill_from_buffer(buffer_iterator start, 
	buffer_iterator end) 
-> buffer_iterator
{
	auto diff = std::distance(start, end);
	if(diff < static_cast<ptrdiff_t>(distribution.min()))
		throw not_enough_data();
	auto iter_end = start + std::min(static_cast<ptrdiff_t>(distribution.max()), diff);
	data.assign(start, iter_end);
	return iter_end;
}