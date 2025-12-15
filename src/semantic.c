#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"

#define IS_INPUT(a) (strcmp(a, "input") == 0)
#define IS_OUTPUT(a) (strcmp(a, "output") == 0)

SymbolNode *get_symbol(SymbolNode *tree, SymbolTable *table, ScopeStack *stack)
{
    SymbolNode *ref_node = NULL;
    ScopeStack *ref_stack = stack;

    while (ref_stack != NULL)
    {
        ref_node = lookup_symbol(table, tree->text, ref_stack->scope);

        if (ref_node != NULL)
            return ref_node;

        ref_stack = ref_stack->parent;
    }

    return NULL;
}

void verify_var_declaration(SymbolNode *tree, SymbolTable *table, ScopeStack *stack)
{
    int is_function = -1, is_array = -1, is_builtin_function = -1;
    SymbolNode *ref_node = NULL, *parent_node = NULL;

    ref_node = get_symbol(tree, table, stack);

    is_builtin_function = (IS_INPUT(tree->text) || IS_OUTPUT(tree->text));

    if (ref_node == NULL && !is_builtin_function)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: Váriavel não definida %s LINHA: %d", tree->text, tree->line);
        exit(1);
    }

    if (is_builtin_function && ((tree->meta & IS_FUNCTION) != IS_FUNCTION))
    {
        fprintf(stderr, "ERRO SEMÂNTICO: Uso incorreto da variável %s LINHA: %d", tree->text, tree->line);
        exit(1);
    }
    else if (is_builtin_function)
        return;

    parent_node = tree->parent;

    is_array = ref_node->meta & IS_ARRAY;
    is_function = ref_node->meta & IS_FUNCTION;

    if (parent_node->type == NODE_CALL && is_function != IS_FUNCTION)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: Váriavel definida %s não é uma função LINHA: %d", tree->text, tree->line);
        exit(1);
    }
    else if (parent_node->type == NODE_VAR && parent_node->children[1] != NULL && is_array != IS_ARRAY)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: Váriavel definida %s não é um array LINHA: %d", tree->text, tree->line);
        exit(1);
    }
}

void verify_expression(SymbolNode *tree, SymbolTable *table, ScopeStack *stack)
{
}

void analyze_semantic(SymbolNode *tree, SymbolTable *table, ScopeStack *stack)
{
    if (tree == NULL)
        return;

    if (tree->type == NODE_PROGRAM && lookup_symbol(table, "main", 0) == NULL)
    {
        fprintf(stderr, "ERRO SEMÂNTICO: Função main não foi definida LINHA: %d", tree->line);
        exit(1);
    }

    stack = scope_stack_push(stack, tree->scope);

    for (int i = 0; i < 4; i++)
        analyze_semantic(tree->children[i], table, stack);

    if (tree->type == NODE_ID && (tree->meta & IS_DECLARATION) != IS_DECLARATION)
        verify_var_declaration(tree, table, stack);
    else if (tree->type == NODE_EXPRESSION)
        verify_expression(tree, table, stack);
    else if (tree->type == NODE_CALL)
    {
    }

    scope_stack_pop(stack);
}