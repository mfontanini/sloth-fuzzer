#ifndef FUZZER_NODES_H
#define FUZZER_NODES_H

#include <cstddef>
#include <functional>
#include "field.h"
#include "field_mapper.h"
#include "block_field.h"

namespace grammar {
template<typename Ret>
class generic_node {
public:
    typedef std::function<Ret(const field_mapper&)> function_type;

    generic_node(function_type function)
    : function(std::move(function))
    {
        
        
    }

    Ret allocate(const field_mapper &mapper)
    {
        return function(mapper);
    }
private:
    function_type function;
};
    
typedef generic_node<field::filler_type> filler_node;
typedef generic_node<field> field_node;

inline field_node *make_block_node(filler_node *filler, size_t size) 
{
    return new field_node(
        [=](const field_mapper &mapper) {
            return field::from_impl<block_field_impl>(filler ? filler->allocate(mapper) : 0, size);
        }
    );
}
}

#endif // FUZZER_NODES_H
