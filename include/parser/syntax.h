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

#ifndef FUZZER_SYNTAX_H
#define FUZZER_SYNTAX_H

#include <map>
#include <string>

#ifndef YYLTYPE_IS_DECLARED
#define YYLTYPE_IS_DECLARED

struct YYLTYPE  
{  
  int first_line;  
  int first_column;  
  int last_line;  
  int last_column;  
};

#endif

#ifndef YYTOKENTYPE
# define YYTOKENTYPE

enum fuzzer_identifiers {
    TEMPLATE = 258,
    IDENTIFIER,
    INT_CONST,
    TEMPLATES,
    BLOCK,
    COMPOUND_BLOCK,
    VAR_BLOCK,
    STR_CONST,
    ERROR,
    COMPOUND_BITFIELD,
    BITFIELD,
    STR_BLOCK,
    CHOICE_FIELD
};
#endif

#endif // FUZZER_SYNTAX_H
