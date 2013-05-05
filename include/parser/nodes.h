#ifndef FUZZER_NODES_H
#define FUZZER_NODES_H

#include <cstddef>
#include <functional>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include "field.h"
#include "field_mapper.h"
#include "function_nodes.h"

class syntax_parser;

namespace grammar {
template<typename Ret, typename... Args>
class generic_node {
public:
    typedef std::function<Ret(field_mapper&, Args...)> function_type;

    generic_node(function_type function)
    : function(std::move(function))
    {
        
        
    }

    template<typename... Ts>
    Ret allocate(field_mapper &mapper, Ts&&... args) const
    {
        return function(mapper, std::forward<Ts>(args)...);
    }
private:
    function_type function;
};
    
typedef generic_node<field::filler_type> filler_node;
typedef generic_node<std::unique_ptr< ::value_node>> value_node;
typedef generic_node<field> field_node;
typedef generic_node<field, size_t, size_t> template_def_node;
typedef std::vector<field_node*> fields_list;
typedef std::map<std::string, field_node*> templates_map;

struct script {
    void add_field_node(field_node *f) 
    {
        fields.push_back(f);
    }
    
    void add_template_node(std::string name, field_node *f)
    {
        templates.insert(std::make_pair(std::move(name), f));
    }

    fields_list fields;
    templates_map templates;
};

} // namespace grammar

#endif // FUZZER_NODES_H
