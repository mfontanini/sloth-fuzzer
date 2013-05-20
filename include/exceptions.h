/*
 * Copyright (c) 2012, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

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

class constraint_too_soft : public std::exception {
public:
    const char *what() const throw();
};

class no_constraints : public std::exception {
public:
    const char *what() const throw();
};

class too_many_constraints : public std::exception {
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

class cant_deduct_field_type : public semantic_exception {
public:
    cant_deduct_field_type(size_t line, const std::string &node);
};

#endif // FUZZER_EXCEPTIONS_H
