#include <openssl/md5.h>
#include <openssl/sha.h>
#include <array>
#include <vector>
#include <algorithm>
#include <cassert>
#include "functions/hashing.h"
#include "field.h"
#include "exceptions.h"
#include "field_mapper.h"
#include "utils.h"

md5_function::md5_function(identifier_type id) 
: unary_field_filler_function<md5_function>(id)
{
    
}

void md5_function::apply(const field &input_field, field &output_field) 
{
    #ifdef FUZZER_DEBUG
        assert(output_field.size() == MD5_DIGEST_LENGTH);
    #endif
    std::array<unsigned char, MD5_DIGEST_LENGTH> buffer;
    const std::vector<unsigned char> input(input_field.begin(), input_field.end());
    MD5(input.data(), input.size(), buffer.data());
    std::copy(buffer.begin(), buffer.end(), output_field.begin());
}

auto md5_function::get_constraints() -> const std::array<constraint_type, 1> &
{
    static const std::array<constraint_type, 1> constraints{
      { make_unique<required_size_constraint>(MD5_DIGEST_LENGTH) }  
    };
    return constraints;
}


sha1_function::sha1_function(identifier_type id) 
: unary_field_filler_function<sha1_function>(id)
{
    
}

void sha1_function::apply(const field &input_field, field &output_field) 
{
    #ifdef FUZZER_DEBUG
        assert(output_field.size() == SHA_DIGEST_LENGTH);
    #endif
    std::array<unsigned char, SHA_DIGEST_LENGTH> buffer;
    const std::vector<unsigned char> input(input_field.begin(), input_field.end());
    SHA1(input.data(), input.size(), buffer.data());
    std::copy(buffer.begin(), buffer.end(), output_field.begin());
}

auto sha1_function::get_constraints() -> const std::array<constraint_type, 1> &
{
    static const std::array<constraint_type, 1> constraints{
      { make_unique<required_size_constraint>(SHA_DIGEST_LENGTH) }  
    };
    return constraints;
}

