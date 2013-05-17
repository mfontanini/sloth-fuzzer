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

#include <sstream>
#include <iterator>
#include "parser/nodes.h"
#include "functions/random.h"
#include "const_value_node.h"
#include "function_nodes.h"
#include "function_value_filler.h"
#include "block_field.h"
#include "template_field.h"
#include "variable_block_field.h"
#include "exceptions.h"

namespace grammar {
field::filler_type default_filler() 
{
    return make_unique< ::random_function>();
}

field::filler_type default_bit_filler() 
{
    return make_unique< ::bitrandom_function>();
}

const std::string compound_field_node_tag::str_repr("multi_block");
const std::string compound_bitfield_node_tag::str_repr("multi_bit");
const std::string choice_field_node_tag::str_repr("choice");

// script

void script::add_field_node(field_node *f)
{
    fields.push_back(f);
}
    
void script::check_constraints() const 
{
    std::for_each(
        fields.begin(),
        fields.end(),
        [](const field_node* f) { f->check_constraints(); }
    );
}

// const_value_node
const_value_node::const_value_node(double value)
: value(value)
{
    
}
    
auto const_value_node::allocate(field_mapper &mapper) -> return_type
{
    return make_unique< ::const_value_node>(value);
}

// node_value_node
node_value_node::node_value_node(field::identifier_type id)
: id(id)
{
    
}

auto node_value_node::allocate(field_mapper &mapper) -> return_type
{
    return make_unique< ::node_value_function_node>(id);
}

// const_string_node

const_string_node::const_string_node(std::string value)
: value(std::move(value))
{
    
}
    
auto const_string_node::allocate(field_mapper &) const -> return_type
{
    return std::make_shared< ::const_string_node>(value);
}

void const_string_node::check_constraints(const field_node &f) const
{
    if(f.min_size() < value.size())
        throw ast_field_too_small(f.get_line_number(), f.to_string(), value);
}

// function_value_filler_node

function_value_filler_node::function_value_filler_node(value_node *value)
: value(value)
{
    
}
    
auto function_value_filler_node::allocate(field_mapper &mapper) const -> return_type
{
    return make_unique< ::function_value_filler>(value->allocate(mapper));
}

void function_value_filler_node::check_constraints(const field_node &f) const
{
    
}

// field_node

void field_node::set_line_number(size_t line)
{
    line_number = line;
}

size_t field_node::get_line_number() const
{
    return line_number;
}

// block_field_node

block_field_node::block_field_node(filler_node *filler, size_t size, identifier_type id)
: filler(filler), size(size), id(id)
{
    
}

auto block_field_node::allocate(field_mapper &mapper) const -> return_type
{
    return field::from_impl< ::block_field_impl>(
        id,
        filler ? filler->allocate(mapper) : default_filler(), 
        size
    );
}

size_t block_field_node::max_size() const
{
    return size;
}

size_t block_field_node::min_size() const
{
    return size;
}

std::string block_field_node::to_string() const
{
    return "block<" + std::to_string(size) + ">";
}

void block_field_node::check_constraints() const
{
    if(filler)
        filler->check_constraints(*this);
}

// bitfield_node

bitfield_node::bitfield_node(filler_node *filler, size_t size, identifier_type id)
: filler(filler), size(size), id(id)
{
    
}
    
auto bitfield_node::allocate(field_mapper &mapper) const -> return_type
{
    return field::from_impl< ::bitfield_impl>(
        id,
        filler ? filler->allocate(mapper) : default_bit_filler(), 
        size
    );
}

size_t bitfield_node::max_size() const
{
    return size;
}

size_t bitfield_node::min_size() const
{
    return size;
}

auto bitfield_node::storage_type() const -> storage
{
    return storage::bits;
}

std::string bitfield_node::to_string() const
{
    return "bitfield<" + std::to_string(size) + ">"; 
}

void bitfield_node::check_constraints() const
{
    if(filler)
        filler->check_constraints(*this);
}

// varblock_field_node

varblock_field_node::varblock_field_node(filler_node *filler, size_t min_sz, 
  size_t max_sz, identifier_type id)
: filler(filler), min_sz(min_sz), max_sz(max_sz), id(id)
{
    
}

auto varblock_field_node::allocate(field_mapper &mapper) const -> return_type
{
    return field::from_impl< ::variable_block_field_impl>(
            id, 
            filler ? filler->allocate(mapper) : default_filler(), 
            min_sz,
            max_sz
    );
}

size_t varblock_field_node::max_size() const
{
    return max_sz;
}

size_t varblock_field_node::min_size() const
{
    return min_sz;
}

std::string varblock_field_node::to_string() const
{
    std::ostringstream oss;
    oss << "var_block<" << min_sz << ", " << max_sz << ">";
    return oss.str();
}

void varblock_field_node::check_constraints() const
{
    if(filler)
        filler->check_constraints(*this);
}

// template_def_node

template_def_node::template_def_node(fields_list *fields)
: fields(fields)
{
    
}

field template_def_node::allocate(field_mapper &mapper, size_t min, size_t max) const
{
    std::vector<field> allocated;
    for(const auto &i : *fields)
        allocated.push_back(i->allocate(mapper));
    auto compound_impl = make_unique< ::compound_field_impl>(
        std::make_move_iterator(allocated.begin()),
        std::make_move_iterator(allocated.end())
    );
    auto impl = make_unique< ::template_field_impl>(
                    field(nullptr, std::move(compound_impl)), 
                    min, 
                    max
                );
    return field(nullptr, std::move(impl));
}

// template_field_node

template_field_node::template_field_node(template_def_node* definition, 
  size_t min_sz, size_t max_sz)
: definition(definition), min_sz(min_sz), max_sz(max_sz)
{
    
}

auto template_field_node::allocate(field_mapper &mapper) const -> return_type
{
    return definition->allocate(mapper, min_sz, max_sz);
}

std::string template_field_node::to_string() const
{
    return "template"; // add name
}
}
