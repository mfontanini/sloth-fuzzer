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

#ifndef FUZZER_SYNTAX_PARSER_H
#define FUZZER_SYNTAX_PARSER_H

#include <memory>
#include <map>
#include <type_traits>
#include <string>
#include <functional>
#include "parser/nodes.h"
#include "field_mapper.h"
#include "field_filler.h"
#include "utils.h"
#include "multiptr_destructor.h"

class field;

class syntax_parser {
public:
    typedef std::shared_ptr<field_filler> filler_ptr;
    typedef std::function<grammar::filler_node*(field_filler::identifier_type)> filler_fun_type;
    typedef std::function<grammar::value_node*(field_filler::identifier_type)> value_fun_type;
    typedef field_filler::identifier_type identifier_type;
    typedef grammar::value_node value_node;
    typedef grammar::field_node field_node;
    typedef grammar::filler_node filler_node;
    typedef grammar::fields_list fields_list;
    typedef grammar::template_def_node template_def_node;

    syntax_parser();

    void parse(const std::string &file_name);
    void parse(std::istream &input);
    void add_template(std::string name, grammar::template_def_node *node);
    field allocate_template(const std::string &name, size_t min, size_t max);
    
    
    void set_script(grammar::script* scr);
    field get_root_field();
    field_mapper &get_mapper();
    
    template<typename Functor>
    void register_filler_function(std::string name, Functor &&ptr)
    {
        filler_functions.insert(std::make_pair(std::move(name), std::move(ptr)));
    }
    
    template<typename Functor>
    void register_value_function(std::string name, Functor &&ptr)
    {
        value_functions.insert(std::make_pair(std::move(name), std::move(ptr)));
    }
    
    bool is_filler_function(const std::string &name);
    bool is_value_function(const std::string &name);
    
    /* ************** Allocator functions ******************/
    
    // block fields
    
    field_node *make_block_node(filler_node *filler, size_t size);
    field_node *make_block_node(filler_node *filler, size_t size, const std::string &name);
    
    // bit field
    
    field_node *make_bitfield_node(value_node *vnode, size_t size);
    field_node *make_bitfield_node(value_node *vnode, size_t size, const std::string &name);
    
    // variable block

    field_node *make_variable_block_node(filler_node *filler, size_t min_size, size_t max_size);
    field_node *make_variable_block_node(filler_node *filler, size_t min_size, size_t max_size, 
      const std::string &name);

    // compound

    field_node *make_compound_field_node(fields_list *fields);
    field_node *make_compound_field_node(fields_list *fields, const std::string &name);
    
    // compound bitfield

    field_node *make_compound_bitfield_node(fields_list *fields);
    field_node *make_compound_bitfield_node(fields_list *fields, const std::string &name);

    // template field

    field_node *make_template_field_node(const std::string &template_name, 
      size_t min, size_t max);

    // template def field

    template_def_node *make_template_def_node(fields_list *fields);
    
    // fields list
    
    fields_list *make_fields_list();
    
    // stuff
    
    value_node *make_const_value_node(double f);
    filler_node *make_const_string_node(const std::string &str);
    value_node *make_node_value_node(const std::string &name); 
    filler_node *make_node_filler_node(const std::string &field_name, 
      const std::string &function_name);
    value_node *make_node_value_function_node(const std::string &field_name, 
      const std::string &function_name);
    filler_node *make_function_value_filler_node(value_node *node);
    
    //
    
    template<typename... Ts>
    std::string *make_string(Ts&&... args)
    {
        return node_alloc<std::string>(std::forward<Ts>(args)...);
    }
    
    grammar::script *make_script();
    
    template<typename Functor>
    value_node *make_binary_function_value_node(value_node *lhs, value_node *rhs) 
    {
        return node_alloc<grammar::binary_value_function_node<Functor>>(lhs, rhs);
    }
private:
    template<bool>
    struct line_number_setter {
        template<typename T>
        static void set(size_t, T*) { }
    };

    template<typename T, typename... Args>
    T *node_alloc(Args&&... args) 
    {
        extern int curr_lineno;
        
        auto smart_ptr = make_unique<T>(std::forward<Args>(args)...);
        auto ptr = smart_ptr.get();
        line_number_setter<std::is_base_of<field_node, T>::value>::set(curr_lineno, ptr);
        destructor.store_pointer(smart_ptr.release());
        return ptr;
    }

    filler_node* allocate_filler_function(const std::string &name, identifier_type id);
    value_node* allocate_value_function(const std::string &name, identifier_type id);
    grammar::script* script_root;
    field_mapper mapper;
    std::map<std::string, filler_fun_type> filler_functions;
    std::map<std::string, value_fun_type> value_functions;
    std::map<std::string, grammar::template_def_node*> templates;
    multiptr_destructor<
        field_node, 
        filler_node, 
        fields_list, 
        template_def_node,
        value_node,
        std::string,
        grammar::script
    > destructor;
};

template<>
struct syntax_parser::line_number_setter<true> {
    template<typename T>
    static void set(size_t line, T *ptr) { 
        ptr->set_line_number(line);
    }
};

#endif // FUZZER_SYNTAX_PARSER_H
