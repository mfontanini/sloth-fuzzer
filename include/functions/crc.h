#ifndef FUZZER_CRC_H
#define FUZZER_CRC_H

#include "field.h"
#include "value_node.h"


class crc32_function : public value_node {
public:
    crc32_function(field::identifier_type id);
    
    double eval(const field_mapper& mapper);
private:
    field::identifier_type id;
};

#endif // FUZZER_CRC_H
