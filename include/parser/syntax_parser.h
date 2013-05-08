#ifndef FUZZER_SYNTAX_PARSER_H
#define FUZZER_SYNTAX_PARSER_H

#include <memory>
#include <map>
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
    typedef grammar::value_node value_node;
    typedef std::shared_ptr<field_filler> filler_ptr;
    typedef std::function<filler_ptr(field_filler::identifier_type)> filler_fun_type;
    typedef std::function<value_node*(field_filler::identifier_type)> value_fun_type;
    typedef field_filler::identifier_type identifier_type;
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
    
    filler_ptr allocate_filler_function(const std::string &name, identifier_type id);
    value_node *allocate_value_function(const std::string &name, identifier_type id);
    bool is_filler_function(const std::string &name);
    bool is_value_function(const std::string &name);
    
    /* ************** Allocator functions ******************/
    
    // block fields
    
    field_node *make_block_node(filler_node *filler, size_t size);
    field_node *make_block_node(filler_node *filler, size_t size, const std::string &name);
    
    // variable block

    field_node *make_variable_block_node(filler_node *filler, size_t min_size, size_t max_size);
    field_node *make_variable_block_node(filler_node *filler, size_t min_size, size_t max_size, 
      const std::string &name);

    // compound

    field_node *make_compound_field_node(fields_list *fields);
    field_node *make_compound_field_node(fields_list *fields, const std::string &name);

    // template field

    field_node *make_template_field_node(const std::string &template_name, 
      size_t min, size_t max);

    // template def field

    template_def_node *make_template_def_node(fields_list *fields);
    
    // fields list
    
    fields_list *make_fields_list();
    
    // stuff
    
    value_node *make_const_value_node(float f);
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
    
    template<typename Functor>
    value_node *make_binary_function_value_node(value_node *lhs, value_node *rhs) 
    {
        return node_alloc<value_node>(
            [=](field_mapper &mapper) {
                return make_unique<Functor>(
                    std::unique_ptr< ::value_node>(lhs->allocate(mapper)),
                    std::unique_ptr< ::value_node>(rhs->allocate(mapper))
                );
            }
        );
    }
private:
    template<typename T, typename... Args>
    T *node_alloc(Args&&... args) 
    {
        auto smart_ptr = make_unique<T>(std::forward<Args>(args)...);
        auto ptr = smart_ptr.get();
        destructor.store_pointer(smart_ptr.release());
        return ptr;
    }

    std::unique_ptr<grammar::script> script_root;
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
        std::string
    > destructor;
};

#endif // FUZZER_SYNTAX_PARSER_H
