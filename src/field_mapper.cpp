#include <functional>
#include "field_mapper.h"

void field_mapper::register_field(std::string name, field::identifier_type id)
{
    str2id.insert(std::make_pair(std::move(name), id));
}

field::identifier_type field_mapper::find_register_field_name(const std::string &fname)
{
    auto iter = str2id.find(fname);
    if(iter == str2id.end())
        iter = str2id.insert({ fname, field::generate_id() }).first;
    return iter->second;
}

void field_mapper::identify_fields(const field &root) 
{
    using std::placeholders::_1;
    
    root.accept_visitor(
        std::bind(&field_mapper::visit, this, _1)
    );
}

void field_mapper::visit(const field &f) 
{
    id2field.insert({ f.id(), f });
}

const field& field_mapper::find_field(field::identifier_type id) const
{
    return id2field.at(id);
}

auto field_mapper::find_field_name(const std::string &fname) const -> field::identifier_type
{
    return str2id.at(fname);
}
