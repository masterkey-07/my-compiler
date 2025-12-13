#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum
{
    NODE_ARGUMENTS,
    NODE_ARGUMENT_LIST,
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

typedef struct TreeNode
{
    int line;
    char *text;
    NodeType type;
    struct TreeNode *sibling;
    struct TreeNode *child[4];
} TreeNode;

TreeNode *parse_syntax_tree(const char *filename);

TreeNode *create_node(NodeType type, int line, const char *text, TreeNode *first_child, TreeNode *second_child, TreeNode *third_child, TreeNode *fourth_child);

void print_symbol_tree(TreeNode *tree, int indent);
#endif