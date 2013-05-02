#ifndef FUZZER_FUNCTION_NODES_H
#define FUZZER_FUNCTION_NODES_H

#include <functional>
#include <algorithm>
#include "value_node.h"

// the Functor is probably a stateless class, so probably EBCO works here
template<typename Functor>
class binary_function_node : public value_node, Functor {
public:
    typedef Functor function_type;
    typedef std::unique_ptr<value_node> unique_node;
    
    binary_function_node(unique_node lhs, unique_node rhs, function_type function = function_type())
    : Functor(std::move(function)), lhs(std::move(lhs)), rhs(std::move(rhs))
    {
        
    }
    
    double eval(const field_mapper& mapper) {
        return (*this)(lhs->eval(mapper), rhs->eval(mapper));
    }
    
    dependents_type dependent_fields() const 
    {
        auto v1 = lhs->dependent_fields(), v2 = rhs->dependent_fields();
        v1.insert(v1.end(), v2.begin(), v2.end());
        return v1;
    }
private:
    unique_node lhs, rhs;
};

template<typename Functor>
class unary_function_node : public value_node, Functor {
public:
    typedef Functor function_type;
    typedef std::unique_ptr<value_node> unique_node;
    
    unary_function_node(unique_node node, function_type function = function_type())
    : Functor(std::move(function)), node(std::move(node))
    {
        
    }
    
    double eval(const field_mapper& mapper) {
        return (*this)(node->eval(mapper));
    }
    
    dependents_type dependent_fields() const 
    {
        return node->dependent_fields();
    }
private:
    unique_node node;
};

typedef binary_function_node<std::plus<float>> plus_function_node;
typedef binary_function_node<std::minus<float>> minus_function_node;
typedef binary_function_node<std::multiplies<float>> multiplies_function_node;
typedef binary_function_node<std::divides<float>> divides_function_node;
typedef binary_function_node<std::modulus<float>> modulus_function_node;
typedef unary_function_node<std::negate<float>> negate_function_node;

#endif // FUZZER_FUNCTION_NODES_H
