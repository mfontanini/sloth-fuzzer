#ifndef FUZZER_BLOCK_VALUE_WRAPPER_H
#define FUZZER_BLOCK_VALUE_WRAPPER_H

#include <vector>
#include "value_wrapper_impl.h"

class block_value_wrapper_impl : 
public clonable_value_wrapper_impl<block_value_wrapper_impl> {
public:
    typedef std::vector<value_type> container_type;

    block_value_wrapper_impl(size_t data_size = 0);

    void set(size_t index, value_type value);
    value_type get(size_t index) const;
    size_t size() const;
private:
    container_type data;
};

#endif // FUZZER_BLOCK_VALUE_WRAPPER_H
