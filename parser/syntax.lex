%{

#include <iostream>
#include "parser/nodes.h"
#include "parser/syntax.h"
#include "parser/syntax_parser.h"
#include "parser/syntax.tab.h"
#define YY_NO_UNPUT

extern int curr_lineno;
extern syntax_parser* grammar_syntax_parser;
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
"var_block"     { return VAR_BLOCK; }
"block"         { return BLOCK; }
"multi_block"   { return COMPOUND_BLOCK; }
"<"             { return yytext[0]; }
">"             { return yytext[0]; }
";"             { return yytext[0]; }
","             { return yytext[0]; }
"="             { return yytext[0]; }
{BRACES}        { return yytext[0]; }
{PARENS}        { return yytext[0]; }
{OPERATOR}      { return yytext[0]; }
{NUMBER}        { 
    yylval.int_val = atoi(yytext); 
    return INT_CONST; 
}
{IDENTIFIER}    { 
    yylval.symbol = grammar_syntax_parser->make_string(yytext); 
    return IDENTIFIER; 
}
[ \t\f\v\r]     // consume whitespace
\n              { curr_lineno++; }
.               { 
    yyterminate();
}

%%
