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

#ifndef FUZZER_FIELD_IMPL_H
#define FUZZER_FIELD_IMPL_H

#include <cstdint>
#include <functional>
#include <vector>
#include "utils.h"

class value_wrapper;
class field;
class field_mapper;
class generation_context;

class field_impl {
public:
    typedef uint8_t value_type;
    typedef std::vector<uint8_t> buffer_type;
    typedef buffer_type::const_iterator buffer_iterator;
    typedef std::function<void(const field &)> visitor_type;
    typedef unsigned int identifier_type;
    typedef std::vector<identifier_type> dependents_type;

    virtual ~field_impl() = default;
    
    virtual std::unique_ptr<field_impl> clone() const = 0;
    virtual void prepare(generation_context &);
    virtual void set(size_t index, value_type value) = 0;
    virtual value_type get(size_t index) const = 0;
    virtual size_t size() const = 0;
    virtual size_t field_count() const;
    virtual void accept_visitor(const visitor_type& visitor) const { };
    virtual dependents_type dependent_fields() const;
    virtual void set_value(int64_t value);
    virtual int64_t get_value() const;
    virtual buffer_iterator fill_from_buffer(buffer_iterator start, buffer_iterator end);
private:

};

// crtp so clone is always defined
template<typename Concrete>
class clonable_field_impl : public field_impl {
public:
    std::unique_ptr<field_impl> clone() const 
    {
        return make_unique<Concrete>(static_cast<const Concrete&>(*this));
    }
};

#endif // FUZZER_FIELD_IMPL_H
