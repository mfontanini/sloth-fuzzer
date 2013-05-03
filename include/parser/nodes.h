#ifndef FUZZER_NODES_H
#define FUZZER_NODES_H

#include <cstddef>
#include <functional>
#include <vector>
#include <string>
#include "field.h"
#include "field_mapper.h"
#include "block_field.h"
#include "value_node.h"
#include "utils.h"
#include "function_value_filler.h"
#include "const_value_node.h"
#include "function_nodes.h"

namespace grammar {
template<typename Ret>
class generic_node {
public:
    typedef std::function<Ret(field_mapper&)> function_type;

    generic_node(function_type function)
    : function(std::move(function))
    {
        
        
    }

    Ret allocate(field_mapper &mapper) const
    {
        return function(mapper);
    }
private:
    function_type function;
};
    
typedef generic_node<field::filler_type> filler_node;
typedef generic_node<std::unique_ptr< ::value_node>> value_node;
typedef generic_node<field> field_node;

struct script {
    void add_field_node(field_node *f) 
    {
        fields.push_back(std::shared_ptr<field_node>(f));
    }

    std::vector<std::shared_ptr<field_node>> fields;
};

inline field_node *make_block_node(filler_node *filler, size_t size) 
{
    return new field_node(
        [=](field_mapper &mapper) {
            return field::from_impl<block_field_impl>(filler ? filler->allocate(mapper) : 0, size);
        }
    );
}

inline field_node *make_block_node(filler_node *filler, size_t size, field_mapper &mapper, const std::string &name) 
{
    auto id = mapper.find_register_field_name(name);
    return new field_node(
        [=](field_mapper &mapper) {
            auto f = field::from_impl<block_field_impl>(id, filler ? filler->allocate(mapper) : 0, size);
            return f;
        }
    );
}

inline value_node *make_const_value_node(float f) 
{
    return new value_node(
        [=](field_mapper &) {
            return make_unique< ::const_value_node>(f);
        }
    );
}

inline value_node *make_node_value_node(field_mapper &mapper, const std::string &name) 
{
    auto id = mapper.find_register_field_name(name);
    return new value_node(
        [=](field_mapper &) {
            return make_unique< ::node_value_function_node>(id);
        }
    );
}

inline filler_node *make_function_value_filler_node(value_node *node) 
{
    return new filler_node(
        [=](field_mapper &mapper) {
            return make_unique< ::function_value_filler>(
                node->allocate(mapper)
            );
        }
    );
}


template<typename Functor>
inline value_node *make_binary_function_value_node(value_node *lhs, value_node *rhs) 
{
    return new value_node(
        [=](field_mapper &mapper) {
            return make_unique<Functor>(
                std::unique_ptr< ::value_node>(lhs->allocate(mapper)),
                std::unique_ptr< ::value_node>(rhs->allocate(mapper))
            );
        }
    );
}
}

#endif // FUZZER_NODES_H
