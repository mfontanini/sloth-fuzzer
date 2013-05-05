%{

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "parser/nodes.h"
#include "parser/syntax.h"
#include "parser/syntax_parser.h"
#include "function_nodes.h"

int counter = 0;

extern syntax_parser* grammar_syntax_parser;

void yyerror(const char *s)
{
std::cout << BLOCK << std::endl;
    extern int curr_lineno;

    std::cerr << "line " << curr_lineno << ": " \
    << s << " at or near ";
    //std::cerr << yylloc.first_column;
    std::cerr << std::endl;
}

extern "C"
{
        int yyparse(); 
        int yylex();  
        int yywrap()
        {
                return 1;
        }
}

%}

%locations
%error-verbose

%union {
    grammar::field_node* ast_field;
    grammar::filler_node* ast_filler;
    grammar::value_node* ast_value_node;
    grammar::script* ast_script;
    grammar::template_def_node* ast_template_def;
    std::vector<grammar::field_node*> *ast_fields;
    int int_val;
    std::string *symbol;
}

%token TEMPLATE 258 BLOCK 262 TEMPLATES 261 COMPOUND_BLOCK 263 VAR_BLOCK 264
%token '<' '>' ';' '+' '-' '/' '*' '{' '}' '(' ')' ','
%token <symbol> IDENTIFIER 259
%token <int_val> INT_CONST 260

%type <ast_field>  field block_field compound_field var_block template_field
%type <ast_template_def> template_def
%type <ast_script> script
%type <ast_fields> fields templates
%type <ast_value_node> expression
%type <ast_filler> filler filler_function

%left '-' '+'
%left '*' '/'

%start script

%destructor { free($$); } <symbol>
%destructor {  } <int_val>


%%

script: 
    TEMPLATES '{' templates '}' ';' fields {
        auto s = new grammar::script(); 
        for(auto &i : *$6) 
            s->add_field_node(std::move(i)); 
        grammar_syntax_parser->set_script(s); 
    }
    |
    fields { 
        auto s = new grammar::script(); 
        for(auto &i : *$1) 
            s->add_field_node(std::move(i)); 
        grammar_syntax_parser->set_script(s);  
    }
;

templates:
    templates template_def {
        $$ = nullptr;
    }
    |
    template_def {
        $$ = nullptr;
    }
;

template_def:
    IDENTIFIER '=' '{' fields '}' ';' {
        $$ = grammar_syntax_parser->make_template_def_node($4);
        grammar_syntax_parser->add_template(*$1, $$);
    }
;

fields:
    field {
        $$ = grammar_syntax_parser->make_fields_list();
        $$->push_back($1);
    }
    |
    fields field {
        $1->push_back($2);
    }
;

field:
    block_field { $$ = $1; }
    |
    compound_field { $$ = $1; }
    |
    var_block { $$ = $1; } 
    | 
    template_field { $$ = $1; }
;

block_field: 
    BLOCK '<' INT_CONST '>' IDENTIFIER '=' filler ';' { 
        $$ = grammar_syntax_parser->make_block_node($7, $3, *$5);  
    }
    |
    BLOCK '<' INT_CONST '>' IDENTIFIER ';' { 
        $$ = grammar_syntax_parser->make_block_node(nullptr, $3, *$5); 
    }
    |
    BLOCK '<' INT_CONST '>' '=' filler ';' { 
        $$ = grammar_syntax_parser->make_block_node($6, $3);
    }
    |
    BLOCK '<' INT_CONST '>' ';' { 
        $$ = grammar_syntax_parser->make_block_node(nullptr, $3); 
    }
;

var_block:
    VAR_BLOCK '<' INT_CONST ',' INT_CONST '>' IDENTIFIER '=' filler ';' {
        $$ = grammar_syntax_parser->make_variable_block_node($9, $3, $5, *$7); 
    }
    |
    VAR_BLOCK '<' INT_CONST ',' INT_CONST '>' IDENTIFIER ';' {
        $$ = grammar_syntax_parser->make_variable_block_node(nullptr, $3, $5, *$7); 
    }
    |
    VAR_BLOCK '<' INT_CONST ',' INT_CONST '>' '=' filler ';' {
        $$ = grammar_syntax_parser->make_variable_block_node($8, $3, $5); 
    }
    |
    VAR_BLOCK '<' INT_CONST ',' INT_CONST '>' ';' {
        $$ = grammar_syntax_parser->make_variable_block_node(nullptr, $3, $5); 
    }
;

compound_field:
    COMPOUND_BLOCK '=' '{' fields '}' ';' {
        $$ = grammar_syntax_parser->make_compound_field_node($4);
    } 
;

template_field:
    TEMPLATE '<' IDENTIFIER ',' INT_CONST ',' INT_CONST '>' ';' {
        $$ = grammar_syntax_parser->make_template_field_node(*$3, $5, $7);
    }
;

filler:
    expression {
        $$ = grammar_syntax_parser->make_function_value_filler_node($1);
    }
    |
    filler_function {
        $$ = $1;
    }
;

filler_function:
    IDENTIFIER '(' IDENTIFIER ')' {
        $$ = grammar_syntax_parser->make_node_filler_node(*$3, *$1);
    }
;

expression: 
    INT_CONST {
        $$ = grammar_syntax_parser->make_const_value_node($1);
    }
    |
    IDENTIFIER { 
        $$ = grammar_syntax_parser->make_node_value_node(*$1);
    }
    |
    expression '+' expression {
        $$ = grammar_syntax_parser->make_binary_function_value_node<plus_function_node>($1, $3);
    }
    |
    expression '-' expression {
        $$ = grammar_syntax_parser->make_binary_function_value_node<minus_function_node>($1, $3);
    }
    |
    expression '/' expression {
        $$ = grammar_syntax_parser->make_binary_function_value_node<divides_function_node>($1, $3);
    }
    |
    expression '*' expression {
        $$ = grammar_syntax_parser->make_binary_function_value_node<multiplies_function_node>($1, $3);
    }
;

%%

