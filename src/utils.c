#include "lexer.h"

void print_lexer_table(const lexer_table table)
{
    printf("Lexer Table:\n");
    printf("Columns: %d, Rows: %d\n", table.columns, table.rows);

    printf("Characters: ");
    for (int i = 0; i < table.columns; i++)
    {
        printf("%s ", table.characters[i]);
    }
    printf("\n");

    printf("Final states: ");
    for (int i = 0; i < table.rows; i++)
    {
        printf("%s ", table.final_states[i] ? "true" : "false");
    }
    printf("\n");

    printf("Decisions:\n");
    for (int i = 0; i < table.rows; i++)
        for (int j = 0; j < table.columns; j++)
        {
            lexer_decision *decision = (lexer_decision *)&(table.data[i][j]);

            printf("[row %d col %d] next_state: %d, go_forward: %s\n", i, j, decision->next_state, decision->go_forward ? "true" : "false");
        }
}
