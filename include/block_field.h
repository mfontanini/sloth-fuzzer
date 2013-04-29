#ifndef FUZZER_BLOCK_FIELD_H
#define FUZZER_BLOCK_FIELD_H

#include <vector>
#include "field_impl.h"

class block_field_impl : public clonable_field_impl<block_field_impl> {
public:
    typedef std::vector<value_type> container_type;

    block_field_impl(size_t data_size);

    void set(size_t index, value_type value);
    value_type get(size_t index) const;
    size_t size() const;
private:
    container_type data;
};

#endif // FUZZER_BLOCK_FIELD_H
