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
    void prepare(generation_context &);
    void set(size_t index, value_type value);
    value_type get(size_t index) const;
    size_t size() const;
    size_t field_count() const;
    void accept_visitor(const visitor_type& visitor) const;

    void add_field(field child);
    dependents_type dependent_fields() const;
protected:
    void clear_children();
private:
    typedef std::vector<field> fields_type;
    typedef std::map<size_t, std::reference_wrapper<field>> indexes_type;
    
    indexes_type::const_iterator find_index(size_t index) const;
    
    fields_type fields;
    indexes_type indexes;
    mutable indexes_type::const_iterator last_iterator;
    size_t total_size;
};

#endif // FUZZER_COMPOUND_FIELD_H
