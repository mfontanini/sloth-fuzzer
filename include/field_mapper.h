#ifndef FUZZER_FIELD_MAPPER_H
#define FUZZER_FIELD_MAPPER_H

#include <map>
#include <string>
#include <unordered_map>
#include <functional>
#include "field.h"

class field_mapper {
public:
    void register_field(std::string name, field::identifier_type id);
    void identify_fields(const field &root);
    const field& find_field(field::identifier_type id) const;
    field::identifier_type find_field_name(const std::string &fname) const;
    field::identifier_type find_register_field_name(const std::string &fname);
private:
    void visit(const field &f);

    std::unordered_map<std::string, field::identifier_type> str2id;
    std::map<field::identifier_type, std::reference_wrapper<const field>> id2field;
};

#endif // FUZZER_FIELD_MAPPER_H
