#include "exceptions.h"

const char *invalid_field_size::what() const throw() 
{
    return "invalid field size";
}

const char *dependency_loop_error::what() const throw() 
{
    return "dependency loop error";
}

const char *parse_error::what() const throw() 
{
    return "parsing error encountered";
}

const char *unprepared_field::what() const throw() 
{
    return "unprepared field";
}

const char *file_open_exception::what() const throw() 
{
    return "file open exception";
}

const char *not_implemented::what() const throw() 
{
    return "not implemented";
}

const char *value_too_large::what() const throw() 
{
    return "value too large";
}

semantic_exception::semantic_exception(size_t line, const std::string &str)
: std::runtime_error("error in line " + std::to_string(line) + ": " + str)
{

}


execution_exception::execution_exception(const std::string &str)
: std::runtime_error("Execution exception: " + str)
{
    
}

ast_field_too_small::ast_field_too_small(size_t line, const std::string &node, const std::string &content)
: semantic_exception(line, node + " too small to hold \"" + content + "\"")
{
    
}


incorrect_ast_field_size::incorrect_ast_field_size(size_t line, const std::string &node, size_t expected)
: semantic_exception(line, node + " should be " + std::to_string(expected) + " bytes long.")
{
    
}    
