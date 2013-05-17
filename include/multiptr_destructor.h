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

#ifndef FUZZER_MULTIPTR_DESTRUCTOR_H
#define FUZZER_MULTIPTR_DESTRUCTOR_H

#include <type_traits>
#include <tuple>
#include <vector>
#include <memory>

template<typename... Ts>
class multiptr_destructor {
public:
    template<typename T>
    void store_pointer(T *ptr) {
        internal_store<0>(ptr);
    }
private:
    template<typename T>
    using ptr_vector = std::vector<std::unique_ptr<T>>;
    
    template<size_t n>
    using tup_elem = typename std::tuple_element<n, std::tuple<Ts...>>::type;
    
    template<size_t n, typename T>
    using enabler = std::enable_if<std::is_base_of<tup_elem<n>, T>::value, void>;
    
    template<size_t n, typename T>
    using disabler = std::enable_if<!std::is_base_of<tup_elem<n>, T>::value, void>;
    
    template<size_t n, typename T>
    typename disabler<n, T>::type internal_store(T* ptr) {
        internal_store<n+1>(ptr);
    }
    
    template<size_t n, typename T>
    typename enabler<n, T>::type internal_store(T* ptr) {
        std::get<n>(vector_tuple).push_back(std::unique_ptr<T>(ptr));
    }
    
    
    std::tuple<ptr_vector<Ts>...> vector_tuple;
};

#endif // FUZZER_MULTIPTR_DESTRUCTOR_H
