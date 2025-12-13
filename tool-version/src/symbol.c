#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

TreeNode *create_node(NodeType type, int line, int column, const char *text, TreeNode *first_child, TreeNode *second_child, TreeNode *third_node, TreeNode *fourth_node)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

    if (!node)
    {
        fprintf(stderr, "Out of memory in newNode\n");
        exit(1);
    }

    node->type = type;
    node->line = line;
    node->column = column;
    node->text = text ? safe_strcpy(text) : NULL;
    node->child[0] = first_child;
    node->child[1] = second_child;
    node->child[2] = third_node;
    node->child[3] = fourth_node;
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

void print_symbol_tree(TreeNode *tree, int indent)
{
    while (tree)
    {
        for (int i = 0; i < indent; ++i)
            printf("  ");

        printf("%s [%d:%d]", get_type_label(tree->type), tree->line, tree->column);

        if (tree->text)
            printf(" (%s)", tree->text);

        printf("\n");

        for (int i = 0; i < 4; ++i)
        {
            if (tree->child[i])
                print_symbol_tree(tree->child[i], indent + 1);
        }

        tree = tree->sibling;
    }
}