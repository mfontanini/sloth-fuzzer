#ifndef FUZZER_TOPOLOGICAL_SORTER_H
#define FUZZER_TOPOLOGICAL_SORTER_H

#include <vector>
#include <set>
#include "field_impl.h"

class field;

class topological_sorter {
public:
    typedef std::vector<field_impl::identifier_type> fields_type;
    
    fields_type topological_sort(const field &f);
    fields_type topological_sort(const field &f, std::set<field_impl::identifier_type> &unresolved);
private:
    
};

#endif // FUZZER_TOPOLOGICAL_SORTER_H
