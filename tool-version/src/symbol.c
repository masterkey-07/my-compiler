/* symbol.c */
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

TreeNode *create_node(NodeType type, const char *text,
                      TreeNode *first_child, TreeNode *second_child,
                      TreeNode *third_node, TreeNode *fourth_node)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

    if (!node)
    {
        fprintf(stderr, "Out of memory in newNode\n");
        exit(1);
    }

    node->type = type;
    node->text = text ? safe_strcpy(text) : NULL;
    node->child[0] = first_child;
    node->child[1] = second_child;
    node->child[2] = third_node;
    node->child[3] = fourth_node;
    node->sibling = NULL;
    return node;
}

TreeNode *append_sibling_node(TreeNode *first_node, TreeNode *second_node)
{
    if (!first_node)
        return second_node;

    TreeNode *reference_node = first_node;

    while (reference_node->sibling)
        reference_node = reference_node->sibling;

    reference_node->sibling = second_node;

    return first_node;
}

static const char *get_type_label(NodeType node)
{
    switch (node)
    {
    case NODE_PROGRAM:
        return "PROGRAM";
    case NODE_DECL_LIST:
        return "DECL_LIST";
    case NODE_VAR_DECL:
        return "VAR_DECL";
    case NODE_FUN_DECL:
        return "FUN_DECL";
    case NODE_TYPE:
        return "TYPE";
    case NODE_PARAM_LIST:
        return "PARAM_LIST";
    case NODE_PARAM:
        return "PARAM";
    case NODE_COMPOUND:
        return "COMPOUND";
    case NODE_LOCAL_DECLS:
        return "LOCAL_DECLS"; // none
    case NODE_STMT_LIST:
        return "STMT_LIST"; // none
    case NODE_EXPR_STMT:
        return "EXPR_STMT";
    case NODE_SELECTION:
        return "SELECTION"; // none
    case NODE_ITERATION:
        return "ITERATION"; // none
    case NODE_RETURN:
        return "RETURN"; // none
    case NODE_EXPR:
        return "EXPR";
    case NODE_VAR:
        return "VAR";
    case NODE_SIMPLE_EXPR:
        return "SIMPLE_EXPR"; // none
    case NODE_REL_OP:
        return "REL_OP"; // none
    case NODE_SUM_EXPR:
        return "SUM_EXPR"; // none
    case NODE_SUM_OP:
        return "SUM_OP"; // none
    case NODE_TERM:
        return "TERM"; // none
    case NODE_MUL_OP:
        return "MUL_OP"; // none
    case NODE_FACTOR:
        return "FACTOR"; // none
    case NODE_CALL:
        return "CALL"; // none
    case NODE_ARG_LIST:
        return "ARG_LIST"; // none
    case NODE_CONST:
        return "CONST"; // none
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

        printf("%s", get_type_label(tree->type));

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
