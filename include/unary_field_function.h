#ifndef FUZZER_UNARY_FIELD_FUNCTION_H
#define FUZZER_UNARY_FIELD_FUNCTION_H

#include "field.h"
#include "value_node.h"
#include "generation_context.h"

template<typename Concrete>
class unary_field_function : public value_node {
public:
    unary_field_function(identifier_type id)
    : id(id)
    {
        
    }
    
    double eval(generation_context &ctx)
    {
        return static_cast<Concrete&>(*this).apply(ctx.get_mapper().find_field(id));
    }

    dependents_type dependent_fields() const
    {
        return { id };
    }
private:
    // The dispatched method should follow this signature:
    //double apply(const field &input_field);

    identifier_type id;
};

template<typename Concrete>
class unary_field_filler_function : public field_filler {
public:
    unary_field_filler_function(identifier_type id)
    : id(id)
    {
        
    }
    void fill(field &f, generation_context &ctx)
    {
        static_cast<Concrete&>(*this).apply(ctx.get_mapper().find_field(id), f);
    }

    dependents_type dependent_fields() const
    {
        return { id };
    }
private:
    // The dispatched method should follow this signature:
    // void apply(const field &input_field, field &output_field);

    identifier_type id;
};

#endif // FUZZER_UNARY_FIELD_FUNCTION_H
