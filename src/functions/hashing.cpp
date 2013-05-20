/*
 * Copyright (c) 2012, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

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

