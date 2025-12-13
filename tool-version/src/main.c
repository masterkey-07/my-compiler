#include <stdio.h>
#include "symbol.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    TreeNode *tree = parse_syntax_tree(argv[1]);

    if (tree == NULL)
    {
        fprintf(stderr, "ERRO: Falha ao fazer parsing do arquivo\n");
        return 1;
    }

    print_symbol_tree(tree, 4);

    return 0;
}