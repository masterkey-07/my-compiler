/* symbol.h */
#ifndef AST_H
#define AST_H

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
    NodeType type;
    char *text;                /* nome do identificador, operador, número em string etc */
    struct TreeNode *child[4]; /* até 4 filhos (ajusta se precisar) */
    struct TreeNode *sibling;  /* próximo irmão na lista */
} TreeNode;

/* Cria um novo nó */
TreeNode *newNode(NodeType type, const char *text,
                  TreeNode *c0, TreeNode *c1,
                  TreeNode *c2, TreeNode *c3);

/* Anexa b ao fim da lista de irmãos de a; retorna o primeiro da lista */
TreeNode *appendSibling(TreeNode *a, TreeNode *b);

/* Imprime a árvore com indentação */
void printTree(TreeNode *t, int indent);

#endif
