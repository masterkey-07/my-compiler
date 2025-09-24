#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

int main(int argc, char const *argv[])
{
    FILE *file = fopen("table.csv", "rb");

    lexer_table *table = read_lexer_table(file);

    printf("columns: %d rows: %d\n", table->columns, table->rows);

    for (int row_index = 0; row_index < table->rows; row_index++)
    {
        for (int column_index = 0; column_index < table->columns; column_index++)
        {
            int next_state = table->data[row_index][column_index]->next_state;
            int should_move_foward = table->data[row_index][column_index]->go_forward;
            printf("next_state:%d advance:%d\t", next_state, should_move_foward);
        }

        printf("final_state:%d\n", table->final_states[row_index]);

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

    deallocate_lexem_table(table);

    fclose(file);

    return 0;
}
