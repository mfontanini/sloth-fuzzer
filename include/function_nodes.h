#ifndef FUZZER_FUNCTION_NODES_H
#define FUZZER_FUNCTION_NODES_H

#include <functional>
#include <algorithm>
#include "value_node.h"
#include "field_mapper.h"
#include "generation_context.h"

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
    
    double eval(generation_context &ctx) {
        return (*this)(lhs->eval(ctx), rhs->eval(ctx));
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
    
    double eval(generation_context &ctx) {
        return (*this)(node->eval(ctx));
    }
    
    dependents_type dependent_fields() const 
    {
        return node->dependent_fields();
    }
private:
    unique_node node;
};

class node_value_function_node : public value_node {
public:
    node_value_function_node(field::identifier_type id)
    : id(id) 
    {
        
    }
    
    double eval(generation_context &ctx) {
        return ctx.get_mapper().find_field(id).get_value();
    }
    
    dependents_type dependent_fields() const 
    {
        return { id };
    }
private:
    field::identifier_type id;
};

typedef binary_function_node<std::plus<double>> plus_function_node;
typedef binary_function_node<std::minus<double>> minus_function_node;
typedef binary_function_node<std::multiplies<double>> multiplies_function_node;
typedef binary_function_node<std::divides<double>> divides_function_node;
typedef binary_function_node<std::modulus<double>> modulus_function_node;
typedef unary_function_node<std::negate<double>> negate_function_node;

#endif // FUZZER_FUNCTION_NODES_H
