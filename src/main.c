#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

int main(int argc, char const *argv[])
{
    FILE *file = fopen("table.csv", "rb");

    lexer_table *x = read_lexer_table(file);

    printf("columns: %d rows: %d\n", x->columns, x->rows);

    for (int row_index = 0; row_index < x->rows; row_index++)
    {
        for (int column_index = 0; column_index < x->columns; column_index++)
        {
            printf("next_state:%d advance:%d\t", x->data[row_index][column_index]->next_state, x->data[row_index][column_index]->go_forward);
        }

        printf("final_state:%d\n", x->final_states[row_index]);

        printf("\n");
    }

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
