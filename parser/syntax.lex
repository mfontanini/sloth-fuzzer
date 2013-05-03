%{

#include <iostream>
#include "parser/nodes.h"
#include "parser/syntax.h"
#include "parser/syntax.tab.h"
#define YY_NO_UNPUT

extern int curr_lineno;
extern std::istream *istr; 
extern "C" int yylex(); 
extern "C" int yyerror(const char *); 

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	istr->read( (char*)buf, max_size); \
    result = istr->gcount();
		

%}

%option yylineno

CHARACTER       [a-zA-Z_-]
DIGIT           [0-9]
NUMBER          {DIGIT}+
BRACES          ("{"|"}")
PARENS          ("("|")")
IDENTIFIER      {CHARACTER}({CHARACTER}|{DIGIT})*
OPERATOR        ("+"|"-"|"*"|"/")

/* Actual syntax */

%%

"templates"     { return TEMPLATES; }
"template"      { return TEMPLATE; }
"block"         { std::cout << "block"; return BLOCK; }
"<"             { std::cout << "<"; return yytext[0]; }
">"             { std::cout << ">"; return yytext[0]; }
";"             { return yytext[0]; }
","             { return yytext[0]; }
"="             { std::cout << "="; return yytext[0]; }
{BRACES}        { return yytext[0]; }
{PARENS}        { return yytext[0]; }
{OPERATOR}      { return yytext[0]; }
{NUMBER}        { std::cout << "-" << atoi(yytext) << "-";
    yylval.int_val = atoi(yytext); 
    return INT_CONST; 
}
{IDENTIFIER}    { 
    std::cout << "Identifier\n";
    yylval.symbol = strdup(yytext); 
    return IDENTIFIER; 
}
[ \t\f\v\r]     // consume whitespace
\n              { curr_lineno++; }
.               { 
    std::cout << "aca\n";
    yyterminate();
}

%%
