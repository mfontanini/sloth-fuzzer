#ifndef FUZZER_CRC_H
#define FUZZER_CRC_H

#include "unary_field_function.h"

class crc32_function : public unary_field_function {
public:
    crc32_function(identifier_type id);
    
    double apply(const field &input_field);
};

#endif // FUZZER_CRC_H
