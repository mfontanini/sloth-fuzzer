%{

#include "parser/syntax.h"
#include "parser/syntax.tab.h"
#define YY_NO_UNPUT

extern int curr_lineno;

%}

CHARACTER       [a-zA-Z_-]
DIGIT           [0-9]
NUMBER          {DIGIT}+
TYPE_DEF_ARGS   ("<"|">")
BRACES          ("{"|"}")
PARENS          ("("|")")
IDENTIFIER      {CHARACTER}({CHARACTER}|{DIGIT})+
OPERATOR        ("+"|"-"|"*"|"/")

/* Actual syntax */

%%

"templates"     { return TEMPLATES; }
"template"      { return TEMPLATE; }
"block"         { return BLOCK; }
{TYPE_DEF_ARGS} { return yytext[0]; }
","             { return yytext[0]; }
"="             { return yytext[0]; }
{BRACES}        { return yytext[0]; }
{PARENS}        { return yytext[0]; }
{OPERATOR}      { return yytext[0]; }
{NUMBER}        { 
    yylval.symbol = strdup(yytext); 
    return INT_CONST; 
}
IDENTIFIER      { 
    yylval.symbol = strdup(yytext); 
    return IDENTIFIER; 
}
<<EOF>>         { yyterminate(); }
\n              { curr_lineno++; }
.               { 
    yyterminate();
}

%%
