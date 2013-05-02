#ifndef FUZZER_FUNCTION_MD5_H
#define FUZZER_FUNCTION_MD5_H

#include "field_filler.h"
#include "field.h"

class md5_function : public field_filler {
public:
    md5_function(field::identifier_type id);
    
    void fill(field &f, const field_mapper& mapper);
private:
    field::identifier_type id;
};

#endif // FUZZER_FUNCTION_MD5_H
