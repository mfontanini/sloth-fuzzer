#include <map>
#include <set>
#include <algorithm>
#include <iostream> // borrame
#include "field.h"
#include "topological_sorter.h"


auto topological_sorter::topological_sort(const field &f) -> fields_type
{
    typedef field_impl::identifier_type identifier_type;
    std::map<identifier_type, field::dependents_type> dependents;
    std::map<identifier_type, std::set<field::identifier_type>> topo_map;
    fields_type result;
    f.accept_visitor(
        [&](const field &a_field) { 
            dependents.insert(std::make_pair(a_field.id(), a_field.dependent_fields()));
            for(const auto& i : dependents[a_field.id()]) {
                topo_map[i].insert(a_field.id());
            }
        }
    );
    
    typedef decltype(topo_map)::const_reference ref_type;
    auto comparer = [](ref_type lhs, ref_type rhs) { return lhs.second.size() < rhs.second.size(); };
    while(topo_map.size()) {
        auto min = *min_element(topo_map.begin(), topo_map.end(), comparer);
        std::cout << min.first << " - " << min.second.size() << std::endl;
        result.push_back(min.first);
        for(const auto &elem : dependents[min.first]) {
            topo_map[elem].erase(min.first);
        }
        topo_map.erase(min.first);
    }
    
    return result;
}
