#ifndef FUZZER_CRC_H
#define FUZZER_CRC_H

#include "value_node.h"

class field;

class crc32_function : public value_node {
public:
    crc32_function(field &input_field);
    
    double eval();
private:
    field &input_field;
};

#endif // FUZZER_CRC_H
