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

#ifndef FUZZER_COMPOUND_FIELD_H
#define FUZZER_COMPOUND_FIELD_H

#include <vector>
#include <map>
#include <functional>
#include "field_impl.h"
#include "field.h"

class compound_field_impl : public clonable_field_impl<compound_field_impl> {
public:
    compound_field_impl();
    compound_field_impl(const compound_field_impl &f);
    compound_field_impl(compound_field_impl&&) = default;
    compound_field_impl& operator=(compound_field_impl f);
    
    void prepare(generation_context &);
    void set(size_t index, value_type value);
    value_type get(size_t index) const;
    size_t size() const;
    size_t field_count() const;
    void accept_visitor(const visitor_type& visitor) const;

    void add_field(field child);
    dependents_type dependent_fields() const;
    
    friend void swap(compound_field_impl &lhs, compound_field_impl &rhs);
protected:
    void clear_children();
private:
    typedef std::vector<field> fields_type;
    typedef std::map<size_t, field*> indexes_type;
    
    indexes_type::const_iterator find_index(size_t index) const;
    
    fields_type fields;
    indexes_type indexes;
    mutable indexes_type::const_iterator last_iterator;
    size_t total_size;
};

#endif // FUZZER_COMPOUND_FIELD_H
