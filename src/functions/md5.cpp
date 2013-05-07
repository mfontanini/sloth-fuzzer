#include <openssl/md5.h>
#include <array>
#include <vector>
#include <algorithm>
#include "functions/md5.h"
#include "field.h"
#include "exceptions.h"
#include "field_mapper.h"

md5_function::md5_function(identifier_type id) 
: unary_field_filler_function<md5_function>(id)
{
    
}

void md5_function::apply(const field &input_field, field &output_field) 
{
    if(output_field.size() < MD5_DIGEST_LENGTH)
        throw invalid_field_size();
    std::array<unsigned char, MD5_DIGEST_LENGTH> buffer;
    const std::vector<unsigned char> input(input_field.begin(), input_field.end());
    MD5(input.data(), input.size(), buffer.data());
    std::copy(buffer.begin(), buffer.end(), output_field.begin());
}
