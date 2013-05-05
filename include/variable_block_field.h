#ifndef FUZZER_VARIABLE_BLOCK_FIELD_H
#define FUZZER_VARIABLE_BLOCK_FIELD_H

#include <vector>
#include <random>
#include "field_impl.h"

class variable_block_field_impl : public clonable_field_impl<variable_block_field_impl> {
public:
    typedef std::vector<value_type> container_type;

    variable_block_field_impl(size_t min_size, size_t max_size);

    void set(size_t index, value_type value);
    value_type get(size_t index) const;
    size_t size() const;
    void prepare(generation_context &context);
private:
    container_type data;
    std::uniform_int_distribution<size_t> distribution;
};

#endif // FUZZER_VARIABLE_BLOCK_FIELD_H
