#include <map>
#include <set>
#include <algorithm>
#include <iostream> //borrame
#include "field.h"
#include "topological_sorter.h"
#include "exceptions.h"

auto topological_sorter::topological_sort(const field &f) -> fields_type
{
    typedef field_impl::identifier_type identifier_type;
    std::map<identifier_type, std::set<field::identifier_type>> topo_map;
    fields_type result;
    f.accept_visitor(
        [&](const field &a_field) { 
            auto vec = a_field.dependent_fields();
            for(const auto &i: vec)
                std::cout << a_field.id() << " depends on " << i << std::endl;
            topo_map[a_field.id()] = decltype(topo_map)::mapped_type(vec.begin(), vec.end());
        }
    );
    
    typedef decltype(topo_map)::const_reference ref_type;
    auto comparer = [](ref_type lhs, ref_type rhs) { return lhs.second.size() < rhs.second.size(); };
    while(topo_map.size()) {
        auto min = *min_element(topo_map.begin(), topo_map.end(), comparer);
        if(!min.second.empty())
            throw dependency_loop_error();
        result.push_back(min.first);
        for(auto &elem : topo_map) {
            elem.second.erase(min.first);
        }
        topo_map.erase(min.first);
    }
    
    return result;
}
