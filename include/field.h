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

#ifndef FUZZER_FIELD_H
#define FUZZER_FIELD_H

#include <type_traits>
#include <iterator>
#include <memory>
#include <cstdint>
#include <atomic>
#include <vector>
#include <cstdint>
#include "utils.h"
#include "field_impl.h"
#include "field_filler.h"

template<typename ValueType>
class field_iterator;

template<typename ValueType>
class dereference_helper;

class field_mapper;
class generation_context;

class field {
public:
    typedef field_impl::identifier_type identifier_type;
    typedef field_impl::value_type value_type;
    typedef field_impl::visitor_type visitor_type;
    typedef field_iterator<value_type> iterator;
    typedef field_iterator<const value_type> const_iterator;
    typedef std::unique_ptr<field_impl> unique_impl;
    typedef std::shared_ptr<field_filler> filler_type;
    typedef field_impl::dependents_type dependents_type;
    
    static constexpr identifier_type invalid_id = 0;

    field(filler_type filler, unique_impl impl);
    field(identifier_type id, filler_type filler, unique_impl impl);
    field(const field &rhs);
    field(field &&rhs) noexcept;
    
    field& operator=(field rhs);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    
    size_t size() const;
    // the number of children fields 
    size_t field_count() const;
    void prepare(generation_context &context);
    void fill(generation_context &context);
    
    void accept_visitor(const visitor_type &visitor) const;
    
    void set_value(int64_t value);
    int64_t get_value() const;
    
    dependents_type dependent_fields() const;
    
    identifier_type id() const {
        return identifier;
    }
    
    dereference_helper<value_type> operator[](size_t index);
    dereference_helper<const value_type> operator[](size_t index) const;
    
    // swap free function
    friend void swap(field &lhs, field &rhs);
    
    template<typename Impl, typename... Args>
    static field from_impl(filler_type filler, Args&&... args) 
    {
        return field(filler, make_unique<Impl>(std::forward<Args>(args)...));
    }
    
    template<typename Impl, typename... Args>
    static field from_impl(identifier_type id, filler_type filler, Args&&... args) 
    {
        return field(id, filler, make_unique<Impl>(std::forward<Args>(args)...));
    }
    
    static identifier_type generate_id();
private:
    static std::atomic<identifier_type> global_id;
    

    unique_impl impl;
    filler_type filler;
    identifier_type identifier;
};

// comparison functions
bool operator<(const field &lhs, const field &rhs);

template<typename ValueType>
class dereference_helper {
public:
    typedef ValueType value_type;
    typedef typename std::conditional<
                std::is_const<ValueType>::value, 
                const field_impl&,
                field_impl&
            >::type wrapped_type;

    dereference_helper(wrapped_type impl, size_t index)
    : impl(impl), index(index)
    {
        
    }
    
    template<typename U = ValueType>
    typename std::enable_if<!std::is_const<U>::value, value_type>::type 
    operator=(value_type value)
    {
        impl.set(index, value);
        return value;
    }
    
    operator value_type() const {
        return impl.get(index);
    }
private:
    wrapped_type impl;
    size_t index;
};

template<typename ValueType>
class field_iterator : 
public std::iterator<std::random_access_iterator_tag, ValueType> {
public:
    field_iterator(field_impl &wrapper, size_t index = 0)
    : wrapper(wrapper), index(index)
    {
        
    }
    
    field_iterator& operator++()
    {
        index++;
        return *this;
    }
    
    field_iterator operator++(int)
    {
        field_iterator copied(*this);
        index++;
        return copied;
    }
    
    field_iterator& operator--()
    {
        index--;
        return *this;
    }
    
    dereference_helper<ValueType> operator*() 
    {
        return dereference_helper<ValueType>(wrapper, index);
    }
    
    dereference_helper<ValueType> operator*() const
    {
        return dereference_helper<ValueType>(wrapper, index);
    }
    
    field_iterator operator--(int)
    {
        field_iterator copied(*this);
        index--;
        return copied;
    }
    
    size_t operator-(const field_iterator& rhs) 
    {
        return index - rhs.index;
    }
    
    field_iterator operator-(size_t offset)
    {
        return field_iterator(wrapper, index - offset);
    }
    
    field_iterator operator+(size_t offset) 
    {
        return field_iterator(wrapper, index + offset);
    }
    
    bool operator==(const field_iterator &rhs) const {
        return index == rhs.index && &wrapper.get() == &rhs.wrapper.get();
    }
    
    bool operator!=(const field_iterator &rhs) const {
        return !(*this == rhs);
    }
private:
    std::reference_wrapper<field_impl> wrapper;
    size_t index;
};


#endif // FUZZER_FIELD_H
