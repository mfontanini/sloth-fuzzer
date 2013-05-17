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

#include <functional>
#include "field_mapper.h"

field::identifier_type field_mapper::register_field(std::string name)
{
    // CHECK
    auto iter = str2id.find(name);
    if(iter != str2id.end()) {
        // this field has already been registered by someone else
        if(iter->second.second)
            throw std::runtime_error("field \"" + name + "\" already defined");
        else {
            iter->second.second = true;
            return iter->second.first;
        }
    }
    
    auto id = field::generate_id();
    str2id[std::move(name)] = { id , true };
    return id;
}

void field_mapper::register_field(field::identifier_type id, const field &f)
{
    id2field.insert({ id, f });
}

field::identifier_type field_mapper::find_register_field_name(const std::string &fname)
{
    auto iter = str2id.find(fname);
    if(iter == str2id.end())
        iter = str2id.insert({ fname, { field::generate_id(), false } }).first;
    return iter->second.first;
}

void field_mapper::identify_fields(const field &root) 
{
    using std::placeholders::_1;
    
    root.accept_visitor(
        std::bind(&field_mapper::visit, this, _1)
    );
}

void field_mapper::visit(const field &f) 
{
    id2field.insert({ f.id(), f });
}

const field& field_mapper::find_field(field::identifier_type id) const
{
    return id2field.at(id);
}

auto field_mapper::find_field_name(const std::string &fname) const -> field::identifier_type
{
    return str2id.at(fname).first;
}
