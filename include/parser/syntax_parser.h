#ifndef FUZZER_SYNTAX_PARSER_H
#define FUZZER_SYNTAX_PARSER_H

#include <memory>
#include "parser/nodes.h"
#include "field_mapper.h"

class syntax_parser {
public:
    void parse(const std::string &file_name);
    void parse(std::istream &input);
    
    void set_script(grammar::script* scr);
    field get_root_field();
    field_mapper &get_mapper();
private:
    std::unique_ptr<grammar::script> script_root;
    field_mapper mapper;
};

#endif // FUZZER_SYNTAX_PARSER_H
