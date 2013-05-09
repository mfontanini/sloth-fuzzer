#ifndef FUZZER_VALUE_NODE_H
#define FUZZER_VALUE_NODE_H

#include "field_impl.h"

class field_mapper;
class generation_context;

class value_node {
public:
    typedef field_impl::identifier_type identifier_type;
    typedef field_impl::dependents_type dependents_type;

    virtual ~value_node() noexcept = default;
    virtual double eval(generation_context &) = 0;
    virtual dependents_type dependent_fields() const {
        return { };
    }
};

#endif // FUZZER_VALUE_NODE_H
