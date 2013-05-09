#ifndef FUZZER_CONST_VALUE_NODE_H
#define FUZZER_CONST_VALUE_NODE_H

#include <iostream>
#include <string>
#include "value_node.h"
#include "field_filler.h"

class const_value_node : public value_node {
public:
    const_value_node(float value);
    double eval(generation_context &ctx);
private:
    float value;
};

class const_string_node : public field_filler {
public:
    const_string_node(std::string value);
    void fill(field &f, generation_context &ctx);
private:
    std::string value;
};

#endif // FUZZER_CONST_VALUE_NODE_H
