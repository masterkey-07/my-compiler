/* symbol.c */
#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *strdup_safe(const char *s)
{
    if (!s)
        return NULL;
    char *r = (char *)malloc(strlen(s) + 1);
    if (!r)
    {
        fprintf(stderr, "Out of memory in strdup_safe\n");
        exit(1);
    }
    strcpy(r, s);
    return r;
}

TreeNode *newNode(NodeType type, const char *text,
                  TreeNode *c0, TreeNode *c1,
                  TreeNode *c2, TreeNode *c3)
{
    TreeNode *n = (TreeNode *)malloc(sizeof(TreeNode));
    if (!n)
    {
        fprintf(stderr, "Out of memory in newNode\n");
        exit(1);
    }
    n->type = type;
    n->text = text ? strdup_safe(text) : NULL;
    n->child[0] = c0;
    n->child[1] = c1;
    n->child[2] = c2;
    n->child[3] = c3;
    n->sibling = NULL;
    return n;
}

TreeNode *appendSibling(TreeNode *a, TreeNode *b)
{
    if (!a)
        return b;
    TreeNode *p = a;
    while (p->sibling)
        p = p->sibling;
    p->sibling = b;
    return a;
}

static const char *nodeTypeName(NodeType t)
{
    switch (t)
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

void printTree(TreeNode *t, int indent)
{
    while (t)
    {
        for (int i = 0; i < indent; ++i)
            printf("  ");

        printf("%s", nodeTypeName(t->type));
        if (t->text)
            printf(" (%s)", t->text);
        printf("\n");

        for (int i = 0; i < 4; ++i)
        {
            if (t->child[i])
                printTree(t->child[i], indent + 1);
        }

        t = t->sibling;
    }
}
