#include "symbol.h"

int main()
{
    TreeNode *tree = parse_syntax_tree();

    print_symbol_tree(tree, 4);

    return 0;
}