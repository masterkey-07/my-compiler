%code requires {
  #include "symbol.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

extern int yylineno;
extern char* yytext;
extern FILE *yyin;


int yylex(void);

void yyerror() {
  fprintf(stderr, "ERRO SINTÁTICO: \"%s\" LINHA: %d\n", yytext, yylineno);
}

SymbolNode *syntax_tree_root = NULL;
%}

%union {
    int      ival;    /* para NUM */
    char    *sval;    /* para ID */
    SymbolNode *node;   /* para nós da AST */
}

%locations

%token INT VOID IF ELSE WHILE RETURN
%token LE GE EQ NE

%token <sval> ID
%token <ival> NUM

%type <node> program declaration_list declaration
%type <node> var_declaration specifier_type non_void_type
%type <node> function_declaration params param_list param
%type <node> compound_declaration local_declarations statement_list
%type <node> statement expression_declaration selection_declaration iteration_declaration return_declaration
%type <node> expression var simple_expression relational
%type <node> sum_expression term sum factor multiplication
%type <node> call arguments argument_list

%right '='
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc EQ NE LE GE '<' '>'
%left '+' '-'
%left '*' '/'

%start program

%%

program
  : declaration_list
    {
        $$ = create_node(NODE_PROGRAM, @1.first_line, @1.first_column, "program", $1, NULL, NULL, NULL);
        syntax_tree_root = $$;
    }
  ;

declaration_list
  : declaration_list declaration
    { $$ = create_node(NODE_DECLARATION_LIST, @1.first_line, @1.first_column, NULL, $1, $2, NULL, NULL); }
  | declaration
    { $$ = create_node(NODE_DECLARATION_LIST, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  ;

declaration
  : var_declaration
    { $$ = create_node(NODE_DECLARATION, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  | function_declaration
    { $$ = create_node(NODE_DECLARATION, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  ;

var_declaration
  : specifier_type ID ';'
    {
        SymbolNode *node_id = create_node(NODE_ID, @2.first_line, @2.first_column, $2, NULL, NULL, NULL, NULL);
        
        $$ = create_node(NODE_VAR_DECLARATION, @2.first_line, @2.first_column, NULL, $1, node_id, NULL, NULL);
    }
  | specifier_type ID '[' NUM ']' ';'
    {
        char buffer[32];
     
        sprintf(buffer, "%d", $4);
     
        SymbolNode *node_id = create_node(NODE_ID, @2.first_line, @2.first_column, $2, NULL, NULL, NULL, NULL);
     
        SymbolNode *node_size = create_node(NODE_NUM, @4.first_line, @4.first_column, buffer, NULL, NULL, NULL, NULL);
     
        $$ = create_node(NODE_VAR_DECLARATION, @1.first_line, @1.first_column, NULL, $1, node_id, node_size, NULL);
    }
  ;

specifier_type
  : INT
    { $$ = create_node(NODE_TYPE, @1.first_line, @1.first_column, "int", NULL, NULL, NULL, NULL); }
  | VOID
    { $$ = create_node(NODE_TYPE, @1.first_line, @1.first_column, "void", NULL, NULL, NULL, NULL); }
  ;

non_void_type
  : INT
    { $$ = create_node(NODE_TYPE, @1.first_line, @1.first_column, "int", NULL, NULL, NULL, NULL); }
  ;

function_declaration
  : specifier_type ID '(' params ')' compound_declaration
    { 
      SymbolNode *node_id = create_node(NODE_ID, @2.first_line, @2.first_column, $2, NULL, NULL, NULL, NULL);
    
      $$ = create_node(NODE_FUN_DECLARATION, @1.first_line, @1.first_column, NULL, $1, node_id, $4, $6); 
    }
  ;

params
  : param_list
    { $$ = create_node(NODE_PARAMS, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  | VOID
    { $$ = create_node(NODE_VOID, @1.first_line, @1.first_column, NULL, NULL, NULL, NULL, NULL); }
  ;

param_list
  : param_list ',' param
    { $$ = create_node(NODE_PARAM_LIST, @1.first_line, @1.first_column, NULL, $1, $3, NULL, NULL); }
  | param
    { $$ = create_node(NODE_PARAM_LIST, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  ;

param
  : non_void_type ID
    {
        SymbolNode *node_id = create_node(NODE_ID, @2.first_line, @2.first_column, $2, NULL, NULL, NULL, NULL);

        $$ = create_node(NODE_PARAM, @1.first_line, @1.first_column, NULL, $1, node_id, NULL, NULL);
    }
  | non_void_type ID '[' ']'
    {
        SymbolNode *node_id = create_node(NODE_ID, @2.first_line, @2.first_column, $2, NULL, NULL, NULL, NULL);
        
        SymbolNode *empty_array = create_node(NODE_EMPTY_ARRAY, @3.first_line, @3.first_column, NULL, NULL, NULL, NULL, NULL);
        
        $$ = create_node(NODE_PARAM, @2.first_line, @2.first_column, NULL, $1, node_id, empty_array, NULL);
    }
  ;

compound_declaration
  : '{' local_declarations statement_list '}'
    { $$ = create_node(NODE_COMPOUND_DECLARATION, @1.first_line, @1.first_column, NULL, $2, $3, NULL, NULL); }
  ;

local_declarations
  : local_declarations var_declaration
    { $$ = create_node(NODE_LOCAL_DECLARATIONS, @1.first_line, @1.first_column, NULL, $1, $2, NULL, NULL); }
  | /* empty */
    { $$ = NULL; }
  ;

statement_list
  : statement_list statement
    { $$ = create_node(NODE_STATEMENT_LIST, @1.first_line, @1.first_column, NULL, $1, $2, NULL, NULL); }
  | /* empty */
    { $$ = NULL; }
  ;

statement
  : expression_declaration       
    { $$ = create_node(NODE_STATEMENT, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  | compound_declaration         
    { $$ = create_node(NODE_STATEMENT, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  | selection_declaration        
    { $$ = create_node(NODE_STATEMENT, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  | iteration_declaration        
    { $$ = create_node(NODE_STATEMENT, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  | return_declaration           
    { $$ = create_node(NODE_STATEMENT, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  ;

expression_declaration
  : expression ';'
    { $$ = create_node(NODE_EXPRESSION_STATEMENT, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  | ';'
    { $$ = create_node(NODE_EXPRESSION_STATEMENT, @1.first_line, @1.first_column, NULL, NULL, NULL, NULL, NULL); }
  ;

expression
  : var '=' expression
    { 
      SymbolNode *node_assign = create_node(NODE_ASSIGN, @2.first_line, @2.first_column, "=", NULL, NULL, NULL, NULL);
      
      $$ = create_node(NODE_EXPRESSION, @1.first_line, @1.first_column, NULL, $1, node_assign, $3, NULL); 
    }
  | simple_expression
    { $$ = create_node(NODE_EXPRESSION, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
  ;

var
  : ID
    {
        SymbolNode *node_id = create_node(NODE_ID, @1.first_line, @1.first_column, $1, NULL, NULL, NULL, NULL);
        
        $$ = create_node(NODE_VAR, @1.first_line, @1.first_column, NULL, node_id, NULL, NULL, NULL);
    }
  | ID '[' expression ']'
    {
        SymbolNode *node_id = create_node(NODE_ID, @1.first_line, @1.first_column, $1, NULL, NULL, NULL, NULL);
        
        $$ = create_node(NODE_VAR, @1.first_line, @1.first_column, NULL, node_id, $3, NULL, NULL);
    }
  ;

selection_declaration:
        IF '(' expression ')' statement %prec LOWER_THAN_ELSE
        { $$ = create_node(NODE_SELECTION_DECLARATION, @1.first_line, @1.first_column, "if", $3, $5, NULL, NULL); }
        | IF '(' expression ')' statement ELSE statement
        { $$ = create_node(NODE_SELECTION_DECLARATION, @1.first_line, @1.first_column, "if_else", $3, $5, $7, NULL); }
        ;

iteration_declaration:
        WHILE '(' expression ')' statement
        { $$ = create_node(NODE_ITERATION_DECLARATION, @1.first_line, @1.first_column, "while", $3, $5, NULL, NULL); }
        ;

return_declaration:
        RETURN ';'
        { $$ = create_node(NODE_RETURN_DECLARATION, @1.first_line, @1.first_column, "return", NULL, NULL, NULL, NULL); }
        | RETURN expression ';'
        { $$ = create_node(NODE_RETURN_DECLARATION, @1.first_line, @1.first_column, "return", $2, NULL, NULL, NULL); }
        ;

simple_expression:
        sum_expression relational sum_expression
          { $$ = create_node(NODE_SIMPLE_EXPRESSION, @1.first_line, @1.first_column, NULL, $1, $2, $3, NULL); }
        | sum_expression
          { $$ = create_node(NODE_SIMPLE_EXPRESSION, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
        ;

relational:
        '<'  
          { $$ = create_node(NODE_RELATION_OPERATION, @1.first_line, @1.first_column, "<",  NULL, NULL, NULL, NULL); }
        | LE  
          { $$ = create_node(NODE_RELATION_OPERATION, @1.first_line, @1.first_column, "<=",  NULL, NULL, NULL, NULL); }
        | '>'
          { $$ = create_node(NODE_RELATION_OPERATION, @1.first_line, @1.first_column, ">",  NULL, NULL, NULL, NULL); }
        | GE
          { $$ = create_node(NODE_RELATION_OPERATION, @1.first_line, @1.first_column, ">=",  NULL, NULL, NULL, NULL); }
        | EQ
          { $$ = create_node(NODE_RELATION_OPERATION, @1.first_line, @1.first_column, "==",  NULL, NULL, NULL, NULL); }
        | NE
          { $$ = create_node(NODE_RELATION_OPERATION, @1.first_line, @1.first_column, "!=",  NULL, NULL, NULL, NULL); }
        ;

sum_expression:
        sum_expression sum term
          { $$ = create_node(NODE_SUM_EXPRESSION, @1.first_line, @1.first_column, NULL, $1, $2, $3, NULL); }
        | term
          { $$ = create_node(NODE_SUM_EXPRESSION, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
        ;

sum
    : '+'
      { $$ = create_node(NODE_SUM_OPERATION, @1.first_line, @1.first_column, "+", NULL, NULL, NULL, NULL); }
    | '-'
      { $$ = create_node(NODE_SUM_OPERATION, @1.first_line, @1.first_column, "-", NULL, NULL, NULL, NULL); }
    ;

term:
        term multiplication factor
          { $$ = create_node(NODE_TERM, @1.first_line, @1.first_column, NULL, $1, $2, $3, NULL); }
        | factor
          { $$ = create_node(NODE_TERM, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
        ;

multiplication
    : '*'
      { $$ = create_node(NODE_MULTIPLICATION_OPERATION, @1.first_line, @1.first_column, "*", NULL, NULL, NULL, NULL); }
    | '/'
      { $$ = create_node(NODE_MULTIPLICATION_OPERATION, @1.first_line, @1.first_column, "/", NULL, NULL, NULL, NULL); }
    ;

factor:
        '(' expression ')'
          { $$ = create_node(NODE_FACTOR, @1.first_line, @1.first_column, NULL, $2, NULL, NULL, NULL); }
        | var
          { $$ = create_node(NODE_FACTOR, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
        | call
          { $$ = create_node(NODE_FACTOR, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL); }
        | NUM
          { 
            char buffer[32];
            sprintf(buffer, "%d", $1);
            SymbolNode *node_num = create_node(NODE_NUM, @1.first_line, @1.first_column, buffer, NULL, NULL, NULL, NULL);

            $$ = create_node(NODE_FACTOR, @1.first_line, @1.first_column, NULL, node_num, NULL, NULL, NULL);
          }
        ;

call:
        ID '(' arguments ')'
          {
            SymbolNode *node_id = create_node(NODE_ID, @1.first_line, @1.first_column, $1, NULL, NULL, NULL, NULL);
            $$ = create_node(NODE_CALL, @3.first_line, @3.first_column, NULL, node_id, $3, NULL, NULL );
          }
        ;

arguments:
        argument_list
        { $$ = create_node(NODE_ARGUMENTS, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL ); }
        | /* empty */
        { $$ = NULL; }
        ;

argument_list:
        argument_list ',' expression
        { $$ = create_node(NODE_ARGUMENT_LIST, @1.first_line, @1.first_column, NULL, $1, $3, NULL, NULL ); }
        | expression
        { $$ = create_node(NODE_ARGUMENT_LIST, @1.first_line, @1.first_column, NULL, $1, NULL, NULL, NULL ); }
        ;
%%

SymbolNode *parse_syntax_tree(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr, "ERRO ARQUIVO: não foi possível abrir o arquivo '%s'\n", filename);
        return NULL;
    }

    yyin = file;

    if (yyparse() == 0) {
        fclose(file);

        return syntax_tree_root;
    }

    fclose(file);

    return NULL;
}