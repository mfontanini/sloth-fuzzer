%{

#include <iostream>
#include "parser/nodes.h"
#include "parser/syntax.h"
#include "parser/syntax_parser.h"
#include "parser/syntax.tab.h"
#define MAX_STR_CONST 1025

extern int curr_lineno;
extern syntax_parser* grammar_syntax_parser;
extern std::istream *istr; 
extern "C" int yylex(); 
extern "C" int yyerror(const char *); 

char string_buf[MAX_STR_CONST]; 
char *string_buf_ptr;

#define CHECK_STRLEN \
    if(string_buf_ptr == string_buf + MAX_STR_CONST - 1) { \
        yylval.error_msg = "String constant too long"; \
        BEGIN(STR_LITERAL_ERR); \
        return ERROR; \
    }

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	istr->read( (char*)buf, max_size); \
    result = istr->gcount();
		

%}

%option yylineno
%option nounput

CHARACTER       [a-zA-Z_-]
DIGIT           [0-9]
NUMBER          {DIGIT}+
BRACES          ("{"|"}")
PARENS          ("("|")")
IDENTIFIER      {CHARACTER}({CHARACTER}|{DIGIT})*
OPERATOR        ("+"|"-"|"*"|"/")

%x SINGLE_COMMENT
%x STR_LITERAL
%x STR_LITERAL_ERR

/* Actual syntax */

%%

"#"                    { BEGIN(SINGLE_COMMENT); }
<SINGLE_COMMENT>[^\n]+  // Eat
<SINGLE_COMMENT>\n      { BEGIN(INITIAL); curr_lineno++; }

<INITIAL>"\""   { 
    string_buf_ptr = string_buf;
    BEGIN(STR_LITERAL); 
}
<STR_LITERAL>"\"" { 
    BEGIN(INITIAL); 
    *string_buf_ptr = 0;
    yylval.symbol = grammar_syntax_parser->make_string(string_buf, string_buf_ptr); 
    return STR_CONST;
}
<STR_LITERAL>"\\n"  { CHECK_STRLEN; *string_buf_ptr++ = '\n'; }
<STR_LITERAL>"\\b"  { CHECK_STRLEN; *string_buf_ptr++ = '\b'; }
<STR_LITERAL>"\\f"  { CHECK_STRLEN; *string_buf_ptr++ = '\f'; }
<STR_LITERAL>"\\t"  { CHECK_STRLEN; *string_buf_ptr++ = '\t'; }
<STR_LITERAL>"\\\\" { CHECK_STRLEN; *string_buf_ptr++ = '\\'; }
<STR_LITERAL>"\\\"" { CHECK_STRLEN; *string_buf_ptr++ = '"';  }
<STR_LITERAL><<EOF>> {  
    yylval.error_msg = "EOF in string constant"; 
    BEGIN(INITIAL);
    return ERROR;
}
<STR_LITERAL>\n { 
    curr_lineno++;
    yylval.error_msg = "Unterminated string constant"; 
    BEGIN(INITIAL);
    return ERROR; 
}
<STR_LITERAL>\0  { 
    yylval.error_msg = "String contains null character"; 
    BEGIN(STR_LITERAL_ERR);
    return ERROR;
}
<STR_LITERAL>"\\x"{DIGIT}{2}  { CHECK_STRLEN; *string_buf_ptr++ = ((yytext[2] - '0') << 4) | (yytext[3] - '0'); }
<STR_LITERAL>"\\\n"  { CHECK_STRLEN; *string_buf_ptr++ = '\n'; curr_lineno++; }
<STR_LITERAL>"\\".   { CHECK_STRLEN; *string_buf_ptr++ = yytext[1]; }
<STR_LITERAL>[^\\\n\"\0]+  { 
    const char* yy_ptr = yytext;
    if(yyleng > (MAX_STR_CONST - 1) - (string_buf_ptr - string_buf)) {
        yylval.error_msg = "String constant too long";
        BEGIN(STR_LITERAL_ERR);
        return ERROR;
    }
    while(*yy_ptr)
        *string_buf_ptr++ = *yy_ptr++; 
}

<STR_LITERAL_ERR>["\n]       { BEGIN(INITIAL); }
<STR_LITERAL_ERR>"\\\""     // Consume
<STR_LITERAL_ERR>\\.        // Consume
<STR_LITERAL_ERR>[^\"\\\n]    // Consume

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
