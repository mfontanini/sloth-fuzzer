#ifndef FUZZER_FUNCTION_SIZE_H
#define FUZZER_FUNCTION_SIZE_H

#include "unary_field_function.h"

template<size_t (field::* MemFun)() const>
class generic_field_function : public unary_field_function {
public:
    generic_field_function(identifier_type id)
    : unary_field_function(id)
    {
        
    }
    
    double apply(const field &input_field)
    {
        return (input_field.*MemFun)();
    }
};

typedef generic_field_function<&field::size> size_function;
typedef generic_field_function<&field::field_count> field_count_function;

#endif // FUZZER_FUNCTION_SIZE_H
