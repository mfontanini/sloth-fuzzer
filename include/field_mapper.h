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

#ifndef FUZZER_FIELD_MAPPER_H
#define FUZZER_FIELD_MAPPER_H

#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include "field.h"

class field_mapper {
public:
    field::identifier_type register_field(std::string name);
    void register_field(field::identifier_type id, const field &f);
    void identify_fields(const field &root);
    const field& find_field(field::identifier_type id) const;
    field::identifier_type find_field_name(const std::string &fname) const;
    field::identifier_type find_register_field_name(const std::string &fname);
    
    template<typename ForwardIterator>
    void find_non_registered_fields(ForwardIterator output)
    {
        std::for_each(
            str2id.begin(),
            str2id.end(),
            [&](const typename decltype(str2id)::value_type& vt) {
                if(!vt.second.second)
                    *output++ = vt.first;
            }
        );
    }
private:
    void visit(const field &f);

    std::unordered_map<std::string, std::pair<field::identifier_type, bool>> str2id;
    std::map<field::identifier_type, std::reference_wrapper<const field>> id2field;
};

#endif // FUZZER_FIELD_MAPPER_H
