#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int context_id = 0;

static char *safe_strcpy(const char *string)
{
    if (!string)
        return NULL;

    char *copy_string = (char *)malloc(strlen(string) + 1);

    if (!copy_string)
    {
        fprintf(stderr, "Out of memory in strdup_safe\n");
        exit(1);
    }

    strcpy(copy_string, string);

    return copy_string;
}

SymbolNode *create_node(NodeType type, int line, int column, const char *text, SymbolNode *first_child, SymbolNode *second_child, SymbolNode *third_node, SymbolNode *fourth_node)
{
    SymbolNode *node = (SymbolNode *)malloc(sizeof(SymbolNode));

    if (!node)
    {
        fprintf(stderr, "Out of memory in newNode\n");
        exit(1);
    }

    node->meta = 0;
    node->type = type;

    node->scope = -1;
    node->line = line;
    node->function = NULL;
    node->column = column;

    node->text = text ? safe_strcpy(text) : NULL;

    node->children[0] = first_child;
    node->children[1] = second_child;
    node->children[2] = third_node;
    node->children[3] = fourth_node;

    node->next = NULL;
    node->sibling = NULL;

    return node;
}

static const char *get_type_label(NodeType node)
{
    switch (node)
    {
    case NODE_PROGRAM:
        return "NODE_PROGRAM";
    case NODE_DECLARATION_LIST:
        return "NODE_DECLARATION_LIST";
    case NODE_DECLARATION:
        return "NODE_DECLARATION";
    case NODE_ID:
        return "NODE_ID";
    case NODE_VAR_DECLARATION:
        return "NODE_VAR_DECLARATION";
    case NODE_NUM:
        return "NODE_NUM";
    case NODE_TYPE:
        return "NODE_TYPE";
    case NODE_FUN_DECLARATION:
        return "NODE_FUN_DECLARATION";
    case NODE_PARAMS:
        return "NODE_PARAMS";
    case NODE_VOID:
        return "NODE_VOID";
    case NODE_PARAM_LIST:
        return "NODE_PARAM_LIST";
    case NODE_PARAM:
        return "NODE_PARAM";
    case NODE_EMPTY_ARRAY:
        return "NODE_EMPTY_ARRAY";
    case NODE_COMPOUND_DECLARATION:
        return "NODE_COMPOUND_DECLARATION";
    case NODE_LOCAL_DECLARATIONS:
        return "NODE_LOCAL_DECLARATIONS";
    case NODE_STATEMENT_LIST:
        return "NODE_STATEMENT_LIST";
    case NODE_STATEMENT:
        return "NODE_STATEMENT";
    case NODE_EXPRESSION_STATEMENT:
        return "NODE_EXPRESSION_STATEMENT";
    case NODE_ASSIGN:
        return "NODE_ASSIGN";
    case NODE_EXPRESSION:
        return "NODE_EXPRESSION";
    case NODE_SIMPLE_EXPRESSION:
        return "NODE_SIMPLE_EXPRESSION";
    case NODE_VAR:
        return "NODE_VAR";
    case NODE_SELECTION_DECLARATION:
        return "NODE_SELECTION_DECLARATION";
    case NODE_ITERATION_DECLARATION:
        return "NODE_ITERATION_DECLARATION";
    case NODE_RETURN_DECLARATION:
        return "NODE_RETURN_DECLARATION";
    case NODE_RELATION_OPERATION:
        return "NODE_RELATION_OPERATION";
    case NODE_SUM_EXPRESSION:
        return "NODE_SUM_EXPRESSION";
    case NODE_SUM_OPERATION:
        return "NODE_SUM_OPERATION";
    case NODE_TERM:
        return "NODE_TERM";
    case NODE_MULTIPLICATION_OPERATION:
        return "NODE_MULTIPLICATION_OPERATION";
    case NODE_FACTOR:
        return "NODE_FACTOR";
    case NODE_CALL:
        return "NODE_CALL";
    case NODE_ARGUMENTS:
        return "NODE_ARGUMENTS";
    case NODE_ARGUMENT_LIST:
        return "NODE_ARGUMENT_LIST";
    default:
        return "UNKNOWN";
    }
}

void print_symbol_table(SymbolTable *table, FILE *out)
{
    if (!table || !out)
        return;

    for (int index = 0; index < TABLE_SIZE; index++)
    {
        SymbolNode *symbol = table->buckets[index];

        if (!symbol)
            continue;

        fprintf(out, "[%03d]:\n", index);

        while (symbol)
        {
            fprintf(out,
                    "\tname: %s\n\t\tline: %d\n\t\tcolumn: %d\n\t\ttype: %s\n\t\tscope: %d\n\t\tis_function: %d\n\t\tfunction_name: %s\n\t\tis_array: %d\n\t\tis_number: %d\n",
                    symbol->text,
                    symbol->line,
                    symbol->column,
                    get_type_label(symbol->type),
                    symbol->scope,
                    MIN(symbol->meta & IS_FUNCTION, 1),
                    symbol->function == NULL ? "global" : symbol->function,
                    MIN(symbol->meta & IS_ARRAY, 1),
                    MIN(symbol->meta & IS_FUNCTION, 1));

            symbol = symbol->next;
        }
    }
}

void print_symbol_tree(SymbolNode *tree, FILE *out, int level)
{
    while (tree)
    {
        for (int index = 0; index < level; ++index)
            fprintf(out, "\t");

        fprintf(out, "%s [%d:%d]", get_type_label(tree->type), tree->line, tree->column);

        if (tree->text)
            fprintf(out, " (%s)", tree->text);

        fprintf(out, "\n");

        for (int index = 0; index < 4; ++index)
            if (tree->children[index])
                print_symbol_tree(tree->children[index], out, level + 1);

        tree = tree->sibling;
    }
}

void map_tree_scope(SymbolNode *tree, int scope)
{
    if (tree->scope == -1)
        tree->scope = scope;

    for (int index = 0; index < 4; index++)
        if (tree->children[index])
            map_tree_scope(tree->children[index], scope);
}

void map_tree_function(SymbolNode *tree, char *function)
{
    tree->function = function;

    for (int index = 0; index < 4; index++)
        if (tree->children[index])
            map_tree_function(tree->children[index], function);
}

SymbolTable *create_symbol_table(void)
{
    SymbolTable *table = malloc(sizeof(SymbolTable));

    for (int index = 0; index < TABLE_SIZE; index++)
        table->buckets[index] = NULL;

    return table;
}

static unsigned long symbol_hash(SymbolNode *symbol)
{
    unsigned long hash = 5381;
    unsigned char character;

    int scope = symbol->scope;
    const char *text = symbol->text;

    while ((character = *text++))
        hash = ((hash << 5) + hash) + character;

    hash = ((hash << 5) + hash) + ':';

    hash = ((hash << 5) + hash) + (symbol->type == NODE_ID ? (unsigned long)scope : (unsigned long)0);

    return hash % TABLE_SIZE;
}

void *lookup_symbol(SymbolTable *table, SymbolNode *symbol)
{
    unsigned long index = symbol_hash(symbol);

    SymbolNode *reference = table->buckets[index];

    while (reference)
    {
        if (strcmp(reference->text, symbol->text) == 0 && (symbol->type == NODE_NUM || reference->scope == symbol->scope))
            return reference;

        reference = reference->next;
    }

    return NULL;
}

void free_symbol_node(SymbolNode *node)
{
    if (!node)
        return;

    for (int index = 0; index < 4; index++)
        if (node->children[index])
            free_symbol_node(node->children[index]);

    if (node->sibling)
        free_symbol_node(node->sibling);

    if (node->text != NULL)
        free(node->text);

    free(node);
}

ScopeStack *scope_stack_push(ScopeStack *past, int scope)
{
    ScopeStack *head = malloc(sizeof(ScopeStack));

    if (!head)
        return past;

    head->past = past;
    head->scope = scope;

    return head;
}

ScopeStack *scope_stack_pop(ScopeStack *stack)
{
    if (!stack)
        return NULL;

    ScopeStack *past = stack->past;

    stack->past = NULL;

    free_context(stack);

    return past;
}

void free_context(ScopeStack *stack)
{
    if (!stack)
        return;

    ScopeStack *reference = stack->past;

    while (reference)
    {
        ScopeStack *past = reference->past;
        free(reference);
        reference = past;
    }

    free(stack);
}

void map_symbol_tree(SymbolTable *table, SymbolNode *tree)
{
    if ((tree->type == NODE_ID && (tree->meta & IS_DECLARATION) == IS_DECLARATION) || tree->type == NODE_NUM)
        insert_symbol(table, tree);

    for (int index = 0; index < 4; index++)
        if (tree->children[index])
            map_symbol_tree(table, tree->children[index]);
}

SymbolTable *build_symbol_table(SymbolNode *tree)
{
    SymbolTable *table = create_symbol_table();

    map_symbol_tree(table, tree);

    return table;
}

void free_symbol_table(SymbolTable *table)
{
    for (int index = 0; index < TABLE_SIZE; index++)
    {
        SymbolNode *node = table->buckets[index];

        while (node)
        {
            SymbolNode *next = node->next;
            free_symbol_node(node);
            node = next;
        }
    }

    free(table);
}

int insert_symbol(SymbolTable *table, SymbolNode *symbol)
{

    unsigned long index = symbol_hash(symbol);

    SymbolNode *reference = table->buckets[index];

    while (reference)
    {
        if (strcmp(reference->text, symbol->text) == 0 && (symbol->type == NODE_NUM || reference->scope == symbol->scope))
            return -1;

        reference = reference->next;
    }

    symbol->next = table->buckets[index];
    table->buckets[index] = symbol;

    return 0;
}

SymbolNode *create_node_id(int line, int column, const char *text, int meta)
{

    SymbolNode *node = create_node(NODE_ID, line, column, text, NULL, NULL, NULL, NULL);

    node->meta = meta;
    node->scope = -1;

    return node;
}
