#ifndef FUZZER_VALUE_NODE_H
#define FUZZER_VALUE_NODE_H

class value_node {
public:
    virtual ~value_node() = default;
    virtual double eval() = 0;
};

#endif // FUZZER_VALUE_NODE_H
