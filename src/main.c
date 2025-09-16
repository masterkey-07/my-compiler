#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

int main(int argc, char const *argv[])
{
    FILE *file = fopen("table.csv", "rb");

    read_lexer_table(file);

    // if (argc != 2)
    //     return 0;

    // char const *file_name = argv[1];

    // FILE *file = fopen(file_name, "rb");

    // if (file == NULL)
    //     return 0;

    // file_buffer *buffer = allocate_buffer(file);

    // printf("%d, %s\n", f.token, f.data);

    // deallocate_buffer(buffer);

    // fclose(file);

    // printf("done\n");

    return 0;
}
