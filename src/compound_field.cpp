#include "compound_field.h"

compound_field_impl::compound_field_impl()
: total_size(0)
{
    
}

void compound_field_impl::add_field(field child)
{
    fields.push_back(std::move(child));
}

auto compound_field_impl::find_index(size_t index) const -> indexes_type::const_iterator
{
    if(index < last_iterator->first)
        last_iterator = indexes.begin();
    auto iter = last_iterator;
    while(iter != indexes.end() && iter->first < index) {
        last_iterator = iter++;
    }
    return last_iterator;
}

void compound_field_impl::prepare(random_generator &) 
{
    indexes.clear();
    size_t index = 0;
    for(auto &child : fields) {
        indexes.insert({ index, child });
        index += child.size();
    }
    last_iterator = indexes.begin();
    total_size = index;
}

void compound_field_impl::set(size_t index, value_type value)
{
    auto iter = find_index(index);
    iter->second.get()[index - iter->first] = value;
}

auto compound_field_impl::get(size_t index) const -> value_type
{
    auto iter = find_index(index);
    return iter->second.get()[index - iter->first];
}

size_t compound_field_impl::size() const
{
    return total_size;
}
