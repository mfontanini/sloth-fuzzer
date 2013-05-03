%{

#include "parser/nodes.h"

%}

%union {
    grammar::field_node* ast_field;
    //filler_node* ast_filler;
    int int_val;
    char *symbol;
}

%token BLOCK
%token <int_val> NUMBER
%token <symbol> INT_CONST IDENTIFIER

%type <ast_field>  field

%%

field: BLOCK '<' NUMBER '>' { $$ = grammar::make_block_node(nullptr, $3); }

%%
