#ifndef FUZZER_FUNCTION_MD5_H
#define FUZZER_FUNCTION_MD5_H

#include "field_filler.h"

class md5_function : public field_filler {
public:
    md5_function(field &input_field);
    
    void fill(field &f);
private:
    field &input_field;
};

#endif // FUZZER_FUNCTION_MD5_H
