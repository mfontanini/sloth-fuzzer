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

#include <cassert>
#include "choice_field.h"
#include "generation_context.h"
#include "utils.h"


void choice_field_impl::set(size_t index, value_type value)
{
    #ifdef FUZZER_DEBUG
        assert(current_choice != nullptr);
    #endif
    (*current_choice)[index] = value;
}

auto choice_field_impl::get(size_t index) const -> value_type
{
    #ifdef FUZZER_DEBUG
        assert(current_choice != nullptr);
    #endif
    return (*current_choice)[index];
}

size_t choice_field_impl::size() const
{
    return current_choice->size();
}

void choice_field_impl::prepare(generation_context &ctx)
{
    std::uniform_int_distribution<size_t> dist(0, choices.size() - 1);
    current_choice = &choices[dist(ctx.get_random_generator())];
    current_choice->prepare(ctx);
    current_choice->fill(ctx);
}

auto choice_field_impl::dependent_fields() const -> dependents_type
{
    return dependents;
}
