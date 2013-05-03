%{

#include <iostream>
#include <vector>
#include <stdio.h>
#include "parser/nodes.h"
#include "parser/syntax.h"
#include "parser/syntax_parser.h"
#include "function_nodes.h"

int counter = 0;

extern syntax_parser* grammar_syntax_parser;

typedef std::vector<grammar::field_node*> fields_list;

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
    std::vector<grammar::field_node*> *ast_fields;
    //filler_node* ast_filler;
    int int_val;
    char *symbol;
}

%token BLOCK 262 TEMPLATES 261
%token '<' '>' ';' '+' '-' '/' '*'
%token <symbol> IDENTIFIER 259
%token <int_val> INT_CONST 260

%type <ast_field>  field
%type <ast_script> script
%type <ast_fields> fields
%type <ast_value_node> expression
%type <ast_filler> filler

%left '-' '+'
%left '*' '/'

%start script

%%

script: 
    fields { 
        auto s = new grammar::script(); 
        for(auto &i : *$1) 
            s->add_field_node(i); 
        grammar_syntax_parser->set_script(s); 
    }
;

fields:
    field {
        $$ = new fields_list();
        $$->push_back($1);
    }
    |
    fields field {
        $1->push_back($2);
    }

field: 
    BLOCK '<' INT_CONST '>' IDENTIFIER '=' filler ';' { 
        $$ = grammar::make_block_node($7, $3, grammar_syntax_parser->get_mapper(), $5);  
    }
    |
    BLOCK '<' INT_CONST '>' IDENTIFIER ';' { 
        $$ = grammar::make_block_node(nullptr, $3, grammar_syntax_parser->get_mapper(), $5); 
    }
    |
    BLOCK '<' INT_CONST '>' ';' { 
        $$ = grammar::make_block_node(nullptr, $3); 
    }
;

filler:
    expression {
        $$ = grammar::make_function_value_filler_node($1);
    }
;

expression: 
    INT_CONST {
        $$ = grammar::make_const_value_node($1);
    }
    |
    IDENTIFIER { 
        $$ = grammar::make_node_value_node(grammar_syntax_parser->get_mapper(), $1);
    }
    |
    expression '+' expression {
        $$ = grammar::make_binary_function_value_node<plus_function_node>($1, $3);
    }
    |
    expression '-' expression {
        $$ = grammar::make_binary_function_value_node<minus_function_node>($1, $3);
    }
    |
    expression '/' expression {
        $$ = grammar::make_binary_function_value_node<divides_function_node>($1, $3);
    }
    |
    expression '*' expression {
        $$ = grammar::make_binary_function_value_node<multiplies_function_node>($1, $3);
    }
;

%%

