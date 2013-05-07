#ifndef FUZZER_FUNCTION_MD5_H
#define FUZZER_FUNCTION_MD5_H

#include "unary_field_function.h"

class md5_function : public unary_field_filler_function<md5_function> {
public:
    md5_function(identifier_type id);
    
    void apply(const field &input_field, field &output_field);
};

#endif // FUZZER_FUNCTION_MD5_H
