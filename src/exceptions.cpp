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

const char *constraint_too_soft::what() const throw() 
{
    return "constraint too soft";
}

const char *too_many_constraints::what() const throw() 
{
    return "too many constraints";
}

const char *no_constraints::what() const throw() 
{
    return "no constraints";
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

cant_deduct_field_type::cant_deduct_field_type(size_t line, const std::string &node)
: semantic_exception(line, "cannot deduce type for \"" + node + "\" field")
{
    
}
