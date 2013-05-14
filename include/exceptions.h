#ifndef FUZZER_EXCEPTIONS_H
#define FUZZER_EXCEPTIONS_H

#include <stdexcept>

class invalid_field_size : public std::exception {
public:
    const char *what() const throw();
};

class dependency_loop_error : public std::exception {
public:
    const char *what() const throw();
};

class parse_error : public std::exception {
public:
    const char *what() const throw();
};

class unprepared_field : public std::exception {
public:
    const char *what() const throw();
};

class file_open_exception : public std::exception {
public:
    const char *what() const throw();
};

class not_implemented : public std::exception {
public:
    const char *what() const throw();
};

class value_too_large : public std::exception {
public:
    const char *what() const throw();
};

class semantic_exception : public std::runtime_error {
public:
    semantic_exception(size_t line, const std::string &str);
};

class execution_exception : public std::runtime_error {
public:
    execution_exception(const std::string &str);
};

class ast_field_too_small : public semantic_exception {
public:
    ast_field_too_small(size_t line, const std::string &node, const std::string &content);
};

class incorrect_ast_field_size : public semantic_exception {
public:
    incorrect_ast_field_size(size_t line, const std::string &node, size_t expected);
};

#endif // FUZZER_EXCEPTIONS_H
