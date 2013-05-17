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

#include <map>
#include <set>
#include <algorithm>
#include <iostream> //borrame
#include "field.h"
#include "topological_sorter.h"
#include "exceptions.h"

auto topological_sorter::topological_sort(const field &f) -> fields_type
{
    std::set<field_impl::identifier_type> unresolved;
    auto result = topological_sort(f, unresolved);
    if(!unresolved.empty())
        throw dependency_loop_error();
    return result;
}

auto topological_sorter::topological_sort(const field &f, std::set<field_impl::identifier_type> &unresolved) -> fields_type
{
    typedef field_impl::identifier_type identifier_type;
    std::map<identifier_type, std::set<field::identifier_type>> topo_map;
    fields_type result;
    f.accept_visitor(
        [&](const field &a_field) { 
            auto vec = a_field.dependent_fields();
            topo_map[a_field.id()] = decltype(topo_map)::mapped_type(vec.begin(), vec.end());
        }
    );
    
    typedef decltype(topo_map)::const_reference ref_type;
    auto comparer = [](ref_type lhs, ref_type rhs) { return lhs.second.size() < rhs.second.size(); };
    while(topo_map.size()) {
        auto min = *min_element(topo_map.begin(), topo_map.end(), comparer);
        // store unresolved fields
        for(const auto &i : min.second)
            unresolved.insert(i);
        result.push_back(min.first);
        for(auto &elem : topo_map) {
            elem.second.erase(min.first);
        }
        topo_map.erase(min.first);
    }
    
    return result;
}
