#include <stdio.h>
#include "symbol.h"

void analyze_semantic(SymbolNode *tree, SymbolTable *table)
{
    if (tree == NULL)
        return;

    for (int i = 0; i < 4; i++)
        analyze_semantic(tree->children[i], table);

    if (tree->type == NODE_ID)
    {
        printf("verify: %s\n", tree->text);
    }
    else if (tree->type == NODE_CALL)
    {
        printf("verify: %s\n", tree->text);
    }
    else if (tree->type == NODE_EXPRESSION)
    {
        printf("verify: %s\n", tree->text);
    }
}