#ifndef FUZZER_TEMPLATE_BLOCK_H
#define FUZZER_TEMPLATE_BLOCK_H

#include <random>
#include "compound_field.h"

class template_field_impl : public compound_field_impl {
public:
    template_field_impl(field tmp_field, size_t min, size_t max);
    
    std::unique_ptr<field_impl> clone() const;
    void prepare(generation_context &);
    dependents_type dependent_fields() const;
private:
    field template_field;
    std::vector<field::identifier_type> ordered, unresolved;
    std::vector<field> generated_fields;
    std::uniform_int_distribution<size_t> distr;
};

#endif // FUZZER_TEMPLATE_BLOCK_H
