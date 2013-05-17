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

#ifndef FUZZER_NODES_H
#define FUZZER_NODES_H

#include <cstddef>
#include <functional>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include "field.h"
#include "field_mapper.h"
#include "function_nodes.h"
#include "bitfield.h"
#include "compound_field.h"
#include "choice_field.h"
#include "utils.h"

namespace grammar {
class field_node;
typedef std::vector<field_node*> fields_list;

struct script {
    void add_field_node(field_node *f);
    void check_constraints() const;

    fields_list fields;
};

// *********************************************************************
//                            Value nodes
// *********************************************************************

class value_node {
public:
    typedef std::unique_ptr< ::value_node> return_type;

    virtual ~value_node() = default;
    virtual return_type allocate(field_mapper &mapper) = 0;
};

class const_value_node : public value_node {
public:
    const_value_node(double value);
    return_type allocate(field_mapper &mapper);
private:
    double value;
};

class node_value_node : public value_node {
public:
    node_value_node(field::identifier_type id);
    return_type allocate(field_mapper &mapper);
private:
    field::identifier_type id;
};

template<typename Functor>
class value_function_node : public value_node {
public:
    value_function_node(field::identifier_type id)
    : id(id)
    {
        
    }
    
    return_type allocate(field_mapper &mapper)
    {
        return make_unique<Functor>(id);
    }
private:
    field::identifier_type id;
};

template<typename Functor>
class binary_value_function_node : public value_node {
public:
    binary_value_function_node(value_node* lhs, value_node* rhs)
    : lhs(lhs), rhs(rhs)
    {
        
    }
    
    return_type allocate(field_mapper &mapper)
    {
        return make_unique<Functor>(
            lhs->allocate(mapper),
            rhs->allocate(mapper)
        );
    }
private:
    value_node *lhs, *rhs;
};

// *********************************************************************
//                            Filler nodes
// *********************************************************************

class filler_node {
public:
    typedef field::filler_type return_type;

    virtual ~filler_node() = default;
    
    virtual return_type allocate(field_mapper &mapper) const = 0;
    virtual void check_constraints(const field_node &f) const = 0;
};

class const_string_node : public filler_node {
public:
    const_string_node(std::string value);
    
    return_type allocate(field_mapper &mapper) const;
    void check_constraints(const field_node &f) const;
private:
    std::string value;
};

class function_value_filler_node : public filler_node {
public:
    function_value_filler_node(value_node *value);
    
    return_type allocate(field_mapper &mapper) const;
    void check_constraints(const field_node &f) const;
private:
    value_node* value;
};

template<typename Functor>
class function_filler_node : public filler_node {
public:
    function_filler_node(field::identifier_type id)
    : id(id)
    {
        
    }
    
    return_type allocate(field_mapper &mapper) const
    {
        return std::make_shared<Functor>(id);
    }
    
    void check_constraints(const field_node &f) const 
    {
        for(const auto &c : Functor::get_constraints())
            c->check(f);
    }
private:
    field::identifier_type id;
};

// *********************************************************************
//                             Field nodes
// *********************************************************************

class field_node {
public:
    typedef field return_type;
    typedef field::identifier_type identifier_type;
    enum class storage {
        bytes,
        bits
    };

    virtual ~field_node() = default;
    virtual size_t max_size() const { return 0; }
    virtual size_t min_size() const { return 0; }
    virtual storage storage_type() const { return storage::bytes; }
    virtual std::string to_string() const = 0;
    virtual void check_constraints() const { };
    
    virtual return_type allocate(field_mapper &mapper) const = 0;
    
    void set_line_number(size_t line);
    size_t get_line_number() const;
private:
    size_t line_number;
};

class block_field_node : public field_node {
public:
    block_field_node(filler_node *filler, size_t size, identifier_type id = field::invalid_id);
    
    return_type allocate(field_mapper &mapper) const;
    size_t max_size() const;
    size_t min_size() const;
    std::string to_string() const;
    void check_constraints() const;
private:
    filler_node *filler;
    size_t size;
    field::identifier_type id;
};

class bitfield_node : public field_node {
public:
    bitfield_node(filler_node *filler, size_t size, identifier_type id = field::invalid_id);
    
    return_type allocate(field_mapper &mapper) const;
    size_t max_size() const;
    size_t min_size() const;
    storage storage_type() const;
    std::string to_string() const;
    void check_constraints() const;
private:
    filler_node *filler;
    size_t size;
    field::identifier_type id;
};

class varblock_field_node : public field_node {
public:
    varblock_field_node(filler_node *filler, size_t min_sz, size_t max_sz, 
      identifier_type id = field::invalid_id);
    
    return_type allocate(field_mapper &mapper) const;
    size_t max_size() const;
    size_t min_size() const;
    std::string to_string() const;
    void check_constraints() const;
private:
    filler_node *filler;
    size_t min_sz, max_sz;
    field::identifier_type id;
};

template<typename Impl, typename Tag>
class generic_compound_field_node : public field_node {
public:
    generic_compound_field_node(fields_list *fields, identifier_type id = field::invalid_id);

    return_type allocate(field_mapper &mapper) const;
    size_t max_size();
    size_t min_size();
    std::string to_string() const;
    void check_constraints() const;
private:
    fields_list *fields;
    field::identifier_type id;
};

struct compound_field_node_tag {
    static const std::string str_repr;
};

struct compound_bitfield_node_tag {
    static const std::string str_repr;
};

struct choice_field_node_tag {
    static const std::string str_repr;
};

typedef generic_compound_field_node<
    ::compound_field_impl, 
    compound_field_node_tag> compound_field_node;
    
typedef generic_compound_field_node<
    ::compound_bitfield_impl,
    compound_bitfield_node_tag> compound_bitfield_node;

typedef generic_compound_field_node<
    ::choice_field_impl,
    choice_field_node_tag> choice_field_node;

class template_def_node {
public:
    template_def_node(fields_list *fields);
    
    field allocate(field_mapper &mapper, size_t min, size_t max) const;
private:
    fields_list *fields;
};

class template_field_node : public field_node {
public:
    template_field_node(template_def_node* definition, size_t min_sz, size_t max_sz);
    
    return_type allocate(field_mapper &mapper) const;
    std::string to_string() const;
private:
    template_def_node* definition;
    size_t min_sz, max_sz;
};


template<typename Impl, typename Tag>
generic_compound_field_node<Impl, Tag>::generic_compound_field_node(
  fields_list *fields, identifier_type id)
: fields(fields), id(id)
{
    
}

template<typename Impl, typename Tag>
auto generic_compound_field_node<Impl, Tag>::allocate(field_mapper &mapper) const
-> return_type
{
    std::vector<field> allocated;
    for(const auto &i : *fields)
        allocated.push_back(i->allocate(mapper));
    auto impl = make_unique<Impl>(
        std::make_move_iterator(allocated.begin()),
        std::make_move_iterator(allocated.end())
    );
    return field(id, nullptr, std::move(impl));
}

template<typename Impl, typename Tag>
size_t generic_compound_field_node<Impl, Tag>::max_size() 
{
    return (*std::max_element(
        fields->begin(),
        fields->end(),
        [](const field_node *lhs, const field_node *rhs) { 
            return lhs->max_size() < rhs->max_size();
        }
    ))->max_size();
}

template<typename Impl, typename Tag>
size_t generic_compound_field_node<Impl, Tag>::min_size() 
{
    return (*std::min_element(
        fields->begin(),
        fields->end(),
        [](const field_node *lhs, const field_node *rhs) { 
            return lhs->min_size() < rhs->min_size();
        }
    ))->min_size();
}

template<typename Impl, typename Tag>
std::string generic_compound_field_node<Impl, Tag>::to_string() const 
{
    return Tag::str_repr;
}

template<typename Impl, typename Tag>
void generic_compound_field_node<Impl, Tag>::check_constraints() const
{
    for_each(
        fields->begin(),
        fields->end(),
        [](const field_node* f) { f->check_constraints(); }
    );
}

} // namespace grammar

#endif // FUZZER_NODES_H
