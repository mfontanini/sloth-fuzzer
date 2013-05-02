#ifndef FUZZER_VALUE_NODE_H
#define FUZZER_VALUE_NODE_H

class field_mapper;

class value_node {
public:
    virtual ~value_node() = default;
    virtual double eval(const field_mapper&) = 0;
};

#endif // FUZZER_VALUE_NODE_H
