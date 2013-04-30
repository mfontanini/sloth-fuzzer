#include <openssl/md5.h>
#include <array>
#include <vector>
#include <algorithm>
#include "functions/md5.h"
#include "field.h"
#include "exceptions.h"

md5_function::md5_function(field &input_field) 
: input_field(input_field)
{
    
}

void md5_function::fill(field &f) 
{
    if(f.size() < MD5_DIGEST_LENGTH)
        throw invalid_field_size();
    std::array<unsigned char, MD5_DIGEST_LENGTH> buffer;
    const std::vector<unsigned char> input(input_field.begin(), input_field.end());
    MD5(input.data(), input.size(), buffer.data());
    std::copy(buffer.begin(), buffer.end(), f.begin());
}
