#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

int main(int argc, char const *argv[])
{
    FILE *table_file = fopen("table.csv", "rb");

    if (table_file == NULL)
        return 0;

    lexer_table *table = read_lexer_table(table_file);

    if (argc != 2)
        return 0;

    char const *file_name = argv[1];

    FILE *file = fopen(file_name, "rb");

    if (file == NULL)
        return 0;

    file_buffer *buffer = allocate_buffer(file);

    lexem *x = get_next_lexem(buffer, table);

    while (x != NULL)
    {
        printf("%d - %s\n", x->token, x->data);

        x = get_next_lexem(buffer, table);
    }

    deallocate_buffer(buffer);

    fclose(table_file);

    deallocate_lexem_table(table);

    printf("done\n");
    return 0;
}
