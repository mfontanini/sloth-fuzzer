#include <functional>
#include "field_mapper.h"

field::identifier_type field_mapper::register_field(std::string name)
{
    // CHECK
    auto iter = str2id.find(name);
    if(iter != str2id.end()) {
        // this field has already been registered by someone else
        if(iter->second.second)
            throw std::runtime_error("field \"" + name + "\" already defined");
        else
            return iter->second.first;
    }
    
    auto id = field::generate_id();
    str2id[std::move(name)] = { id , true };
    return id;
}

void field_mapper::register_field(field::identifier_type id, const field &f)
{
    id2field.insert({ id, f });
}

field::identifier_type field_mapper::find_register_field_name(const std::string &fname)
{
    auto iter = str2id.find(fname);
    if(iter == str2id.end())
        iter = str2id.insert({ fname, { field::generate_id(), false } }).first;
    return iter->second.first;
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
    return str2id.at(fname).first;
}
