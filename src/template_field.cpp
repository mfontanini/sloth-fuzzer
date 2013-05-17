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

#include <set>
#include <ctime>
#include "topological_sorter.h"
#include "template_field.h"
#include "generation_context.h"


template_field_impl::template_field_impl(field tmp_field, size_t min, size_t max)
: template_field(tmp_field), distr(min, max)
{
    topological_sorter sorter;
    std::set<field::identifier_type> unresolved_set;
    ordered = sorter.topological_sort(template_field, unresolved_set);
    unresolved.assign(unresolved_set.begin(), unresolved_set.end());
}

void template_field_impl::prepare(generation_context &ctx)
{
    auto num_fields = distr(ctx.get_random_generator());
    clear_children();
    for(auto i = size_t(); i < num_fields; ++i) {
        field f = template_field;
        generation_context local_ctx(std::time(0) ^ reinterpret_cast<time_t>(&f) ^ i);
        field_mapper &mapper = local_ctx.get_mapper();
        mapper.identify_fields(f);
        for(const auto &id : unresolved)
            mapper.register_field(id, ctx.get_mapper().find_field(id));
        for(const auto &id : ordered) {
            auto &q = const_cast<field&>(mapper.find_field(id));
            q.prepare(ctx);
            q.fill(local_ctx);
        }
        add_field(std::move(f));
    }
    
    compound_field_impl::prepare(ctx);
}

auto template_field_impl::dependent_fields() const -> dependents_type
{
    return unresolved;
}

std::unique_ptr<field_impl> template_field_impl::clone() const 
{
    return make_unique<template_field_impl>(*this);
}
