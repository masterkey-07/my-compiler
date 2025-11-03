#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "utils.h"
#include "lexer_table.h"

int main(int argc, char const *argv[])
{
    print_lexer_table(LEXER_TABLE);

    if (argc != 2)
        return 0;

    char const *file_name = argv[1];

    FILE *file = fopen(file_name, "rb");

    if (file == NULL)
        return 1;

    file_buffer *buffer = allocate_buffer(file);

    lexem *lexem_found = get_next_lexem(buffer, LEXER_TABLE);

    while (lexem_found != NULL)
    {
        printf("%d - %s\n", lexem_found->token, lexem_found->data);

        lexem_found = get_next_lexem(buffer, LEXER_TABLE);
    }

    deallocate_buffer(buffer);

    printf("done\n");

    return 0;
}
