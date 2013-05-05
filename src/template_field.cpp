#include <set>
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
        field_mapper mapper;
        mapper.identify_fields(f);
        for(const auto &id : unresolved)
            mapper.register_field(id, ctx.get_mapper().find_field(id));
        for(const auto &id : ordered) {
            auto &q = const_cast<field&>(mapper.find_field(id));
            q.prepare(ctx);
            q.fill(mapper);
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
