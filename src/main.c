#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"
#include "semantic.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    SymbolNode *tree = parse_syntax_tree(argv[1]);

    FILE *tree_file = fopen("symbol_tree.txt", "w+");
    FILE *table_file = fopen("symbol_table.txt", "w+");

    if (tree == NULL)
    {
        fprintf(stderr, "ERRO: Falha ao fazer parsing do arquivo\n");
        return 1;
    }

    SymbolTable *table = build_symbol_table(tree);

    print_symbol_tree(tree, tree_file, 0);

    print_symbol_table(table, table_file);

    analyze_semantic(tree, table);

    return 0;
}