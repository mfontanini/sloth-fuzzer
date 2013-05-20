%{

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include "parser/nodes.h"
#include "parser/syntax.h"
#include "parser/syntax_parser.h"
#include "function_nodes.h"

extern int curr_lineno;
int num_errors = 0;

extern syntax_parser* grammar_syntax_parser;

void yyerror(const char *s)
{
    extern int curr_lineno;

    std::cerr << "error in line " << curr_lineno << ": " 
              << s << std::endl;
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
    uint64_t int_val;
    std::string *symbol;
    const char *error_msg;
}

%token TEMPLATE 258 BLOCK 262 TEMPLATES 261 COMPOUND_BLOCK 263 VAR_BLOCK 264
%token COMPOUND_BITFIELD 267 BITFIELD 268 STR_BLOCK 269 CHOICE_FIELD 270
%token AUTO_FIELD 271
%token '<' '>' ';' '+' '-' '/' '*' '{' '}' '(' ')' ','
%token <symbol> IDENTIFIER 259 STR_CONST 265
%token <int_val> INT_CONST 260

%type <ast_field>  field block_field compound_field var_block template_field 
%type <ast_field> choice_field compound_bitfield bitfield auto_field
%type <ast_template_def> template_def
%type <ast_script> script
%type <ast_fields> fields templates bitfields
%type <ast_value_node> expression expression_func
%type <ast_filler> filler filler_function

%left '-' '+'
%left '*' '/'

%start script


%%

script: 
    TEMPLATES '{' templates '}' ';' fields {
        if(num_errors == 0) {
            auto s = grammar_syntax_parser->make_script(); 
            for(auto &i : *$6) 
                s->add_field_node(std::move(i)); 
            grammar_syntax_parser->set_script(s); 
        }
    }
    |
    fields { 
        if(num_errors == 0) {
            auto s = grammar_syntax_parser->make_script(); 
            for(auto &i : *$1) 
                s->add_field_node(std::move(i)); 
            grammar_syntax_parser->set_script(s);  
        }
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
    IDENTIFIER '{' fields '}' ';' {
        $$ = grammar_syntax_parser->make_template_def_node($3);
        grammar_syntax_parser->add_template(*$1, $$);
    }
;

fields:
    field {
        $$ = grammar_syntax_parser->make_fields_list();
        if($1)
            $$->push_back($1);
    }
    |
    fields field {
        if($2)
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
    |
    compound_bitfield { $$ = $1; }
    |
    choice_field { $$ = $1; }
    |
    auto_field { $$ = $1; }
    |
    error ';' { num_errors++; yyerrok; }
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
    |
    STR_BLOCK '=' STR_CONST ';' {
        $$ = grammar_syntax_parser->make_block_node(
            grammar_syntax_parser->make_const_string_node(*$3), 
            $3->size()
        );
    }
    |
    STR_BLOCK IDENTIFIER '=' STR_CONST ';' {
        $$ = grammar_syntax_parser->make_block_node(
            grammar_syntax_parser->make_const_string_node(*$4), 
            $4->size(),
            *$2
        );
    }
;

auto_field:
    AUTO_FIELD IDENTIFIER '=' filler ';' { 
        $$ = grammar_syntax_parser->make_auto_node($4, *$2);  
    }
    |
    AUTO_FIELD '=' filler ';' { 
        $$ = grammar_syntax_parser->make_auto_node($3);  
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
    COMPOUND_BLOCK '{' fields '}' ';' {
        $$ = grammar_syntax_parser->make_compound_field_node($3);
    } 
    |
    COMPOUND_BLOCK IDENTIFIER '{' fields '}' ';' {
        $$ = grammar_syntax_parser->make_compound_field_node($4, *$2);
    } 
;

compound_bitfield:
    COMPOUND_BITFIELD '{' bitfields '}' ';' {
        $$ = grammar_syntax_parser->make_compound_bitfield_node($3);
    } 
    |
    COMPOUND_BITFIELD IDENTIFIER '{' bitfields '}' ';' {
        $$ = grammar_syntax_parser->make_compound_bitfield_node($4, *$2);
    } 
;

choice_field:
    CHOICE_FIELD '{' fields '}' ';' {
        $$ = grammar_syntax_parser->make_choice_field_node($3);
    } 
    |
    CHOICE_FIELD IDENTIFIER '{' fields '}' ';' {
        $$ = grammar_syntax_parser->make_choice_field_node($4, *$2);
    } 
;


bitfields:
    bitfield {
        $$ = grammar_syntax_parser->make_fields_list();
        $$->push_back($1);
    }
    |
    bitfields bitfield {
        $1->push_back($2);
    }
;

bitfield:
    BITFIELD '<' INT_CONST '>' IDENTIFIER '=' expression ';' { 
        $$ = grammar_syntax_parser->make_bitfield_node($7, $3, *$5);  
    }
    |
    BITFIELD '<' INT_CONST '>' IDENTIFIER ';' { 
        $$ = grammar_syntax_parser->make_bitfield_node(nullptr, $3, *$5); 
    }
    |
    BITFIELD '<' INT_CONST '>' '=' expression ';' { 
        $$ = grammar_syntax_parser->make_bitfield_node($6, $3);
    }
    |
    BITFIELD '<' INT_CONST '>' ';' { 
        $$ = grammar_syntax_parser->make_bitfield_node(nullptr, $3); 
    }
;

template_field:
    TEMPLATE '<' IDENTIFIER ',' INT_CONST ',' INT_CONST '>' ';' {
        $$ = grammar_syntax_parser->make_template_field_node(*$3, $5, $7);
    }
    |
    TEMPLATE '<' IDENTIFIER ',' INT_CONST '>' ';' {
        $$ = grammar_syntax_parser->make_template_field_node(*$3, $5, $5);
    }
    |
    TEMPLATE '<' IDENTIFIER '>' ';' {
        $$ = grammar_syntax_parser->make_template_field_node(*$3, 1, 1);
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
    |
    STR_CONST {
        $$ = grammar_syntax_parser->make_const_string_node(*$1);
    }
;

filler_function:
    IDENTIFIER '(' IDENTIFIER ')' {
        $$ = grammar_syntax_parser->make_node_filler_node(*$3, *$1);
        if($$ == nullptr) {
            yyerror(("function \"" + *$1 + "\" does not exist").c_str());
            YYABORT;
        }
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
    '(' expression ')' {
        $$ = $2;
    }
    |
    expression_func '+' expression_func {
        $$ = grammar_syntax_parser->make_binary_function_value_node<plus_function_node>($1, $3);
    }
    |
    expression_func '-' expression_func {
        $$ = grammar_syntax_parser->make_binary_function_value_node<minus_function_node>($1, $3);
    }
    |
    expression_func '/' expression_func {
        $$ = grammar_syntax_parser->make_binary_function_value_node<divides_function_node>($1, $3);
    }
    |
    expression_func '*' expression_func {
        $$ = grammar_syntax_parser->make_binary_function_value_node<multiplies_function_node>($1, $3);
    }
;

expression_func:
    IDENTIFIER '(' IDENTIFIER ')' {
        $$ = grammar_syntax_parser->make_node_value_function_node(*$3, *$1);
        if($$ == nullptr) {
            yyerror(("function \"" + *$1 + "\" does not exist").c_str());
            YYABORT;
        }
    }
    |
    expression {
        $$ = $1;
    }
;

%%

