#ifndef FUZZER_UNARY_FIELD_FUNCTION_H
#define FUZZER_UNARY_FIELD_FUNCTION_H

#include "field.h"
#include "value_node.h"


class unary_field_function : public value_node {
public:
    unary_field_function(identifier_type id);
    double eval(const field_mapper& mapper);
    dependents_type dependent_fields() const;
private:
    virtual double apply(const field &input_field) = 0;

    identifier_type id;
};

class unary_field_filler_function : public field_filler {
public:
    unary_field_filler_function(identifier_type id);
    void fill(field &f, const field_mapper& mapper);
    dependents_type dependent_fields() const;
private:
    virtual void apply(const field &input_field, field &output_field) = 0;

    identifier_type id;
};

#endif // FUZZER_UNARY_FIELD_FUNCTION_H
