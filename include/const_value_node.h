#ifndef FUZZER_CONST_VALUE_NODE_H
#define FUZZER_CONST_VALUE_NODE_H

#include "value_node.h"

class const_value_node : public value_node {
public:
    const_value_node(float value)
    : value(value)
    {
        
    }
    
    double eval() {
        return value;
    }
private:
    float value;
};

#endif // FUZZER_CONST_VALUE_NODE_H
