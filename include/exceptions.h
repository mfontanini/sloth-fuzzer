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

#endif // FUZZER_EXCEPTIONS_H
