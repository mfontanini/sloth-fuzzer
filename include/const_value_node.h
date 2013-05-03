#ifndef FUZZER_CONST_VALUE_NODE_H
#define FUZZER_CONST_VALUE_NODE_H

#include <iostream>
#include "value_node.h"

class const_value_node : public value_node {
public:
    const_value_node(float value)
    : value(value)
    {
        
    }
    
    double eval(const field_mapper&) {
        return value;
    }
private:
    float value;
};

#endif // FUZZER_CONST_VALUE_NODE_H
