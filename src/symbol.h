#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>

#define TABLE_SIZE 211

#define IS_ARRAY 0b010
#define IS_NORMAL 0b000
#define IS_FUNCTION 0b100
#define IS_DECLARATION 0b001

typedef enum
{
    NODE_ARGUMENT_LIST,
    NODE_ARGUMENTS,
    NODE_ASSIGN,
    NODE_CALL,
    NODE_COMPOUND_DECLARATION,
    NODE_DECLARATION_LIST,
    NODE_DECLARATION,
    NODE_EMPTY_ARRAY,
    NODE_EXPRESSION_STATEMENT,
    NODE_EXPRESSION,
    NODE_FACTOR,
    NODE_FUN_DECLARATION,
    NODE_ID,
    NODE_ITERATION_DECLARATION,
    NODE_LOCAL_DECLARATIONS,
    NODE_MULTIPLICATION_OPERATION,
    NODE_NUM,
    NODE_PARAM_LIST,
    NODE_PARAM,
    NODE_PARAMS,
    NODE_PROGRAM,
    NODE_RELATION_OPERATION,
    NODE_RETURN_DECLARATION,
    NODE_SELECTION_DECLARATION,
    NODE_SIMPLE_EXPRESSION,
    NODE_STATEMENT_LIST,
    NODE_STATEMENT,
    NODE_SUM_EXPRESSION,
    NODE_SUM_OPERATION,
    NODE_TERM,
    NODE_TYPE,
    NODE_VAR_DECLARATION,
    NODE_VAR,
    NODE_VOID,
} NodeType;

typedef struct SymbolNode
{
    int meta;
    int line;
    int column;
    int scope;
    char *text;
    NodeType type;
    char *function;
    struct SymbolNode *next;
    struct SymbolNode *father;
    struct SymbolNode *sibling;
    struct SymbolNode *children[4];
} SymbolNode;

typedef struct ScopeStack
{
    int scope;
    struct ScopeStack *past;
} ScopeStack;

typedef struct SymbolTable
{
    SymbolNode *buckets[TABLE_SIZE];
} SymbolTable;

SymbolNode *parse_syntax_tree(const char *filename);

SymbolNode *create_node_id(int line, int column, const char *text, int meta);

SymbolNode *create_node(NodeType type, int line, int column, const char *text, SymbolNode *first_child, SymbolNode *second_child, SymbolNode *third_child, SymbolNode *fourth_child);

void print_symbol_table(SymbolTable *table, FILE *out);

void print_symbol_tree(SymbolNode *tree, FILE *out, int level);

void map_tree_scope(SymbolNode *tree, int scope);

void map_tree_function(SymbolNode *tree, char *function);

SymbolTable *create_symbol_table(void);

int insert_symbol(SymbolTable *table, SymbolNode *node);

void *lookup_symbol(SymbolTable *table, SymbolNode *symbol);

void free_symbol_table(SymbolTable *table);

void free_symbol_node(SymbolNode *node);

ScopeStack *scope_stack_push(ScopeStack *next, int scope);

ScopeStack *scope_stack_pop(ScopeStack *stack);

void free_context(ScopeStack *stack);

SymbolTable *build_symbol_table(SymbolNode *tree);

#endif