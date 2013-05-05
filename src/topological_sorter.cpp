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
