#ifndef FUZZER_EXCEPTIONS_H
#define FUZZER_EXCEPTIONS_H

#include <stdexcept>

class invalid_field_size : public std::exception {
public:
    const char *what() const throw() {
        return "invalid field size";
    }
};

class dependency_loop_error : public std::exception {
public:
    const char *what() const throw() {
        return "dependency loop error";
    }
};

class parse_error : public std::exception {
public:
    const char *what() const throw() {
        return "parse error";
    }
};

class unprepared_field : public std::exception {
public:
    const char *what() const throw() {
        return "unprepared field";
    }
};

class file_open_exception : public std::exception {
public:
    const char *what() const throw() {
        return "file open exception";
    }
};

class not_implemented : public std::exception {
public:
    const char *what() const throw() {
        return "not implemented";
    }
};

class value_too_large : public std::exception {
public:
    const char *what() const throw() {
        return "value too large";
    }
};

class parser_exception : public std::runtime_error {
public:
    parser_exception(const std::string &str)
    : std::runtime_error("Parser exception: " + str)
    {
        
    }
};

class execution_exception : public std::runtime_error {
public:
    execution_exception(const std::string &str)
    : std::runtime_error("Execution exception: " + str)
    {
        
    }
};

class ast_field_too_small : public parser_exception {
public:
    ast_field_too_small(const std::string &node, const std::string &content)
    : parser_exception(node + " too small to hold: \"" + content + "\"")
    {
        
    }
};

class incorrect_ast_field_size : public parser_exception {
public:
    incorrect_ast_field_size(const std::string &node, size_t expected)
    : parser_exception(node + " should be " + std::to_string(expected) + " bytes long.")
    {
        
    }    
};

#endif // FUZZER_EXCEPTIONS_H
