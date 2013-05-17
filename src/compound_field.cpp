#include <algorithm>
#include "compound_field.h"
#include "exceptions.h"

compound_field_impl::compound_field_impl()
: last_iterator(indexes.end()), total_size(0)
{
    
}

compound_field_impl::compound_field_impl(const compound_field_impl &f)
: fields(f.fields), last_iterator(indexes.end()), total_size(f.total_size)
{
    
}

compound_field_impl& compound_field_impl::operator=(compound_field_impl f)
{
    swap(*this, f);
    return *this;
}

void compound_field_impl::add_field(field child)
{
    fields.push_back(std::move(child));
}

auto compound_field_impl::find_index(size_t index) const -> indexes_type::const_iterator
{
    // prepare must be called before accesing this field
    if(last_iterator == indexes.end())
        throw unprepared_field();
    if(index < last_iterator->first)
        last_iterator = indexes.begin();
    auto iter = last_iterator;
    while(iter != indexes.end() && index >= iter->first) {
        last_iterator = iter++;
    }
    return last_iterator;
}

void compound_field_impl::prepare(generation_context &) 
{
    indexes.clear();
    size_t index = 0;
    for(auto &child : fields) {
        indexes.insert({ index, &child });
        index += child.size();
    }
    last_iterator = indexes.begin();
    total_size = index;
}

void compound_field_impl::set(size_t index, value_type value)
{
    auto iter = find_index(index);
    (*iter->second)[index - iter->first] = value;
}

auto compound_field_impl::get(size_t index) const -> value_type
{
    auto iter = find_index(index);
    return (*iter->second)[index - iter->first];
}

size_t compound_field_impl::size() const
{
    return total_size;
}

size_t compound_field_impl::field_count() const
{
    return fields.size();
}

void compound_field_impl::accept_visitor(const visitor_type& visitor) const
{
    for(const auto &f : fields) {
        f.accept_visitor(visitor);
    }
}

auto compound_field_impl::dependent_fields() const -> dependents_type
{
    dependents_type output;
    transform(
        fields.begin(),
        fields.end(),
        std::back_inserter(output),
        std::mem_fn(&field::id)
    );
    return output;
}

void compound_field_impl::clear_children()
{
    fields.clear();
}

void swap(compound_field_impl &lhs, compound_field_impl &rhs) 
{
    using std::swap;
    
    swap(lhs.fields, rhs.fields);
    swap(lhs.indexes, rhs.indexes);
    swap(lhs.last_iterator, rhs.last_iterator);
    swap(lhs.total_size, rhs.total_size);
}
