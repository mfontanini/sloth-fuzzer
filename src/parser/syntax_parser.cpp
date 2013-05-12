#include <fstream>
#include <iostream> // borrame
#include "parser/syntax_parser.h"
#include "compound_field.h"
#include "exceptions.h"
#include "field.h"
#include "functions/md5.h"
#include "functions/misc.h"
#include "functions/crc.h"
#include "functions/random.h"
#include "block_field.h"
#include "bitfield.h"
#include "compound_field.h"
#include "template_field.h"
#include "variable_block_field.h"
#include "function_value_filler.h"
#include "const_value_node.h"


std::istream *istr = nullptr;
int curr_lineno;
syntax_parser* grammar_syntax_parser = nullptr;


extern "C" int yyparse();

field::filler_type default_filler() {
    return make_unique<random_function>();
}

field::filler_type default_bit_filler() {
    return make_unique<bitrandom_function>();
}

syntax_parser::syntax_parser()
{
    register_filler_function(
        "md5", 
        [](identifier_type id) { return std::make_shared<md5_function>(id); }
    );
    register_value_function(
        "size", 
        [&](identifier_type id) { 
            return node_alloc<value_node>([=](field_mapper &) {
                return make_unique<size_function>(id);
            }); 
        }
    );
    register_value_function(
        "count", 
        [&](identifier_type id) {
            return node_alloc<value_node>([=](field_mapper &) {
                return make_unique<field_count_function>(id); 
            });
        }
    );
    register_value_function(
        "crc", 
        [&](identifier_type id) {
            return node_alloc<value_node>([=](field_mapper &) {
                return make_unique<crc32_function>(id); 
            });
        }
    );
}

void syntax_parser::parse(const std::string &file_name)
{
    std::ifstream input(file_name);
    parse(input);
}

void syntax_parser::parse(std::istream &input)
{
    istr = &input;
    grammar_syntax_parser = this;
    curr_lineno = 1;
    
    if(yyparse() != 0)        
        throw parse_error();
    grammar_syntax_parser = nullptr;
    istr = nullptr;
}

void syntax_parser::add_template(std::string name, grammar::template_def_node *node)
{
    templates.insert(
        std::make_pair(std::move(name), node)
    );
}

field syntax_parser::allocate_template(const std::string &name, size_t min, size_t max)
{
    return templates.at(name)->allocate(mapper, min, max);
}

void syntax_parser::set_script(grammar::script* scr)
{
    script_root.reset(scr);
}

field syntax_parser::get_root_field()
{
    if(!script_root)
        throw parse_error();
    auto impl = make_unique<compound_field_impl>();
    
    for(const auto& i : script_root->fields)
        impl->add_field(i->allocate(mapper));
    
    return field(nullptr, std::move(impl));
}

field_mapper &syntax_parser::get_mapper()
{
    return mapper;
}

auto syntax_parser::allocate_filler_function(const std::string &name, identifier_type id) -> filler_ptr
{
    return filler_functions.at(name)(id);
}

auto syntax_parser::allocate_value_function(const std::string &name, identifier_type id) -> value_node*
{
    return value_functions.at(name)(id);
}

// block field

auto syntax_parser::make_block_node(filler_node *filler, size_t size) -> field_node *
{
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            return field::from_impl<block_field_impl>(
                filler ? filler->allocate(mapper) : default_filler(), 
                size
            );
        }
    );
}

auto syntax_parser::make_block_node(filler_node *filler, size_t size, 
  const std::string &name) -> field_node *
{
    auto id = mapper.find_register_field_name(name);
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            return field::from_impl<block_field_impl>(
                id, 
                filler ? filler->allocate(mapper) : default_filler(), 
                size
            );
        }
    );
}

// bitfield

auto syntax_parser::make_bitfield_node(filler_node *filler, size_t size) -> field_node *
{
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            return field::from_impl<bitfield_impl>(
                filler ? filler->allocate(mapper) : default_bit_filler(), 
                size
            );
        }
    );
}

auto syntax_parser::make_bitfield_node(filler_node *filler, size_t size, 
  const std::string &name) -> field_node *
{
    auto id = mapper.find_register_field_name(name);
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            return field::from_impl<bitfield_impl>(
                id, 
                filler ? filler->allocate(mapper) : default_bit_filler(), 
                size
            );
        }
    );
}

// variable block

auto syntax_parser::make_variable_block_node(filler_node *filler, 
  size_t min_size, size_t max_size) -> field_node *
{
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            return field::from_impl<variable_block_field_impl>(
                filler ? filler->allocate(mapper) : default_filler(), 
                min_size,
                max_size
            );
        }
    );
}

auto syntax_parser::make_variable_block_node(filler_node *filler, 
  size_t min_size, size_t max_size, const std::string &name) 
-> field_node *
{
    auto id = mapper.find_register_field_name(name);
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            return field::from_impl<variable_block_field_impl>(
                id, 
                filler ? filler->allocate(mapper) : default_filler(), 
                min_size,
                max_size
            );
        }
    );
}

// compound

auto syntax_parser::make_compound_field_node(fields_list *fields) -> field_node*
{
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            auto impl = make_unique< ::compound_field_impl>();
            for(const auto &i : *fields)
                impl->add_field(i->allocate(mapper));
            return field(nullptr, std::move(impl));
        }
    );
}

auto syntax_parser::make_compound_field_node(fields_list *fields, const std::string &name) -> field_node *
{
    auto id = mapper.find_register_field_name(name);
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            auto impl = make_unique< ::compound_field_impl>();
            for(const auto &i : *fields)
                impl->add_field(i->allocate(mapper));
            return field(id, nullptr, std::move(impl));
        }
    );
}

// compound bitfield

auto syntax_parser::make_compound_bitfield_node(fields_list *fields) -> field_node*
{
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            auto impl = make_unique< ::compound_bitfield_impl>();
            for(const auto &i : *fields)
                impl->add_field(i->allocate(mapper));
            return field(nullptr, std::move(impl));
        }
    );
}

auto syntax_parser::make_compound_bitfield_node(fields_list *fields, const std::string &name) -> field_node *
{
    auto id = mapper.find_register_field_name(name);
    return node_alloc<field_node>(
        [=](field_mapper &mapper) {
            auto impl = make_unique< ::compound_bitfield_impl>();
            for(const auto &i : *fields)
                impl->add_field(i->allocate(mapper));
            return field(id, nullptr, std::move(impl));
        }
    );
}

// template field

auto syntax_parser::make_template_field_node(const std::string &template_name, 
  size_t min, size_t max) -> field_node*
{
    auto &parser = *this;
    return node_alloc<field_node>(
        [=, &parser](field_mapper &mapper) {
            return parser.allocate_template(template_name, min, max);
        }
    );
}

// template def field

auto syntax_parser::make_template_def_node(fields_list *fields) -> template_def_node*
{
    return node_alloc<template_def_node>(
        [&, fields](field_mapper &mapper, size_t min, size_t max) {
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
    );
}

auto syntax_parser::make_fields_list() -> fields_list *
{
    return node_alloc<fields_list>();
}

// stuff

auto syntax_parser::make_const_value_node(double f) -> value_node *
{
    return node_alloc<value_node>(
        [=](field_mapper &) {
            return make_unique< ::const_value_node>(f);
        }
    );
}

auto syntax_parser::make_const_string_node(const std::string &str) -> filler_node *
{
    return node_alloc<filler_node>(
        [=](field_mapper &) {
            return std::make_shared<const_string_node>(str);
        }
    );
}

auto syntax_parser::make_node_value_node(const std::string &name) -> value_node *
{
    auto id = mapper.find_register_field_name(name);
    return node_alloc<value_node>(
        [=](field_mapper &) {
            return make_unique< ::node_value_function_node>(id);
        }
    );
}

auto syntax_parser::make_node_filler_node(const std::string &field_name, 
  const std::string &function_name) -> filler_node *
{
    auto id = mapper.find_register_field_name(field_name);
    auto &parser = *this;
    if(is_filler_function(function_name)) {
        return node_alloc<filler_node>(
            [=, &parser](field_mapper &) {
                return allocate_filler_function(function_name, id);
            }
        );
    }
    else if(is_value_function(function_name)) {
        return node_alloc<filler_node>(
            [=, &parser](field_mapper &mapper) {
                return make_unique< ::function_value_filler>(
                    ::function_value_filler::unique_value(
                        allocate_value_function(function_name, id)->allocate(mapper)
                    )
                );
            }
        );
    }
    else {
        return nullptr; // fixme
    }
}

auto syntax_parser::make_node_value_function_node(const std::string &field_name, 
  const std::string &function_name) -> value_node *
{
    auto id = mapper.find_register_field_name(field_name);
    return allocate_value_function(function_name, id);
}

auto syntax_parser::make_function_value_filler_node(value_node *node) -> filler_node *
{
    return node_alloc<filler_node>(
        [=](field_mapper &mapper) {
            return make_unique< ::function_value_filler>(
                node->allocate(mapper)
            );
        }
    );
}

bool syntax_parser::is_filler_function(const std::string &name)
{
    return filler_functions.count(name) == 1;
}

bool syntax_parser::is_value_function(const std::string &name)
{
    return value_functions.count(name) == 1;
}
