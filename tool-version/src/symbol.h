#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum
{
    NODE_PROGRAM,
    NODE_DECL_LIST,
    NODE_VAR_DECL,
    NODE_FUN_DECL,
    NODE_TYPE,
    NODE_PARAM_LIST,
    NODE_PARAM,
    NODE_COMPOUND,
    NODE_LOCAL_DECLS,
    NODE_STMT_LIST,
    NODE_EXPR_STMT,
    NODE_SELECTION,
    NODE_ITERATION,
    NODE_RETURN,
    NODE_EXPR,
    NODE_VAR,
    NODE_SIMPLE_EXPR,
    NODE_REL_OP,
    NODE_SUM_EXPR,
    NODE_SUM_OP,
    NODE_TERM,
    NODE_MUL_OP,
    NODE_FACTOR,
    NODE_CALL,
    NODE_ARG_LIST,
    NODE_CONST,
    NODE_GENERIC,
    NODE_NUM,
    NODE_ID
} NodeType;

typedef struct TreeNode
{
    int line;
    char *text;
    NodeType type;
    struct TreeNode *sibling;
    struct TreeNode *child[4];
} TreeNode;

TreeNode *parse_syntax_tree(void);

TreeNode *create_node(NodeType type, int line, const char *text, TreeNode *first_child, TreeNode *second__child, TreeNode *third_child, TreeNode *fourth_child);

TreeNode *append_sibling_node(TreeNode *first_node, TreeNode *second_node);

void print_symbol_tree(TreeNode *tree, int indent);
#endif