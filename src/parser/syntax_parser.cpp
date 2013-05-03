#include <fstream>
#include <iostream> // borrame
#include "parser/syntax_parser.h"
#include "compound_field.h"
#include "exceptions.h"
#include "utils.h"

std::istream *istr = nullptr;
int curr_lineno = 0;
syntax_parser* grammar_syntax_parser = nullptr;

extern "C" void yyparse();

void syntax_parser::parse(const std::string &file_name)
{
    std::ifstream input(file_name);
    parse(input);
}

void syntax_parser::parse(std::istream &input)
{
    istr = &input;
    grammar_syntax_parser = this;
    
    yyparse();
    
    grammar_syntax_parser = nullptr;
    istr = nullptr;
}

void syntax_parser::set_script(grammar::script* scr)
{
    script_root.reset(scr);
}

field syntax_parser::get_root_field()
{
    if(!script_root)
        throw parse_error();
    auto impl = make_unique<compound_field_impl>();
    
    for(const auto& i : script_root->fields)
        impl->add_field(i->allocate(mapper));
    
    return field(nullptr, std::move(impl));
}

field_mapper &syntax_parser::get_mapper()
{
    return mapper;
}
