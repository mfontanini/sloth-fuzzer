#include "parser/nodes.h"
#include "functions/random.h"
#include "const_value_node.h"
#include "function_nodes.h"
#include "function_value_filler.h"
#include "block_field.h"
#include "bitfield.h"
#include "compound_field.h"
#include "template_field.h"
#include "variable_block_field.h"

namespace grammar {
field::filler_type default_filler() 
{
    return make_unique< ::random_function>();
}

field::filler_type default_bit_filler() 
{
    return make_unique< ::bitrandom_function>();
}

size_t maximum_size(const fields_list &fields) 
{
    return (*std::max_element(
        fields.begin(),
        fields.end(),
        [](const field_node *lhs, const field_node *rhs) { 
            return lhs->max_size() < rhs->max_size();
        }
    ))->max_size();
}

size_t minimum_size(const fields_list &fields) 
{
    return (*std::min_element(
        fields.begin(),
        fields.end(),
        [](const field_node *lhs, const field_node *rhs) { 
            return lhs->min_size() < rhs->min_size();
        }
    ))->min_size();
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

// function_value_filler_node

function_value_filler_node::function_value_filler_node(value_node *value)
: value(value)
{
    
}
    
auto function_value_filler_node::allocate(field_mapper &mapper) const -> return_type
{
    return make_unique< ::function_value_filler>(value->allocate(mapper));
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

// bitfield_node

bitfield_node::bitfield_node(filler_node *filler, size_t size, identifier_type id)
: filler(filler), size(size), id(id)
{
    
}
    
auto bitfield_node::allocate(field_mapper &mapper) const -> return_type
{
    return field::from_impl< ::bitfield_impl>(
        id,
        filler ? filler->allocate(mapper) : default_filler(), 
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

// compound_field_node

compound_field_node::compound_field_node(fields_list *fields, identifier_type id)
: fields(fields), id(id)
{
    
}

auto compound_field_node::allocate(field_mapper &mapper) const -> return_type
{
    auto impl = make_unique< ::compound_field_impl>();
    for(const auto &i : *fields)
        impl->add_field(i->allocate(mapper));
    return field(id, nullptr, std::move(impl));
}

size_t compound_field_node::max_size() const
{
    return maximum_size(*fields);
}

size_t compound_field_node::min_size() const
{
    return minimum_size(*fields);
}

// compound_bitfield_node

compound_bitfield_node::compound_bitfield_node(fields_list *fields, identifier_type id)
: fields(fields), id(id)
{
    
}
    
auto compound_bitfield_node::allocate(field_mapper &mapper) const -> return_type
{
    auto impl = make_unique< ::compound_bitfield_impl>();
    for(const auto &i : *fields)
        impl->add_field(i->allocate(mapper));
    return field(id, nullptr, std::move(impl));
}

size_t compound_bitfield_node::max_size() const
{
    return maximum_size(*fields);
}

size_t compound_bitfield_node::min_size() const
{
    return minimum_size(*fields);
}

// template_def_node

template_def_node::template_def_node(fields_list *fields)
: fields(fields)
{
    
}

field template_def_node::allocate(field_mapper &mapper, size_t min, size_t max) const
{
    auto compound_impl = make_unique< ::compound_field_impl>();
    for(const auto &i : *fields)
        compound_impl->add_field(i->allocate(mapper));
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
}
