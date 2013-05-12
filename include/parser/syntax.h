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
    STR_BLOCK
};
#endif

#endif // FUZZER_SYNTAX_H
