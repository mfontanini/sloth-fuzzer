#ifndef FUZZER_BLOCK_FIELD_H
#define FUZZER_BLOCK_FIELD_H

#include "field_impl.h"
#include "value_wrapper.h"

class block_field_impl : public clonable_field_impl<block_field_impl> {
public:
    block_field_impl(size_t size);

    value_wrapper &value();
private:
    value_wrapper v_wrapper;
};

#endif // FUZZER_BLOCK_FIELD_H
