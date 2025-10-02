#include <string.h>

#include "lexer.h"
#include "file_buffer.h"

int get_columns_index(char c, char **columns, int length)
{
    for (int column_index = 0; column_index < length; column_index++)
        if (strcmp(&c, columns[column_index]) == 0)
            return column_index;

    for (int column_index = 0; column_index < length; column_index++)
        if (strcmp(columns[column_index], "space") == 0 && c == ' ')
            return column_index;
        else if (strcmp(columns[column_index], "digit") == 0 && isdigit(c))
            return column_index;
        else if (strcmp(columns[column_index], "char") == 0 && isalpha(c))
            return column_index;
        else if (strcmp(columns[column_index], "linebreak") == 0 && c == '\n')
            return column_index;

    return -1;
}

lexem *get_next_lexem(file_buffer *buffer, const lexer_table table)
{
    char c;
    bool is_final_state;
    lexer_decision *decision;
    int current_state = 0, index = 0, column;

    lexem *lexem_item = allocate_lexem_buffer();

    do
        c = get_next_char(buffer);
    while (c == ' ');

    if (c == EOF)
        return NULL;

    do
    {
        column = get_columns_index(c, table.characters, table.columns);

        if (column == -1)
        {
            printf("decision not found\n");
            exit(1);
        }

        decision = &table.data[current_state][column];

        is_final_state = table.final_states[current_state];

        if (is_final_state)
            lexem_item->token = current_state;

        if (is_final_state == true && decision->next_state == -1)
            return lexem_item;

        if (decision->next_state == -1)
        {
            printf("non final null decision found\n");
            exit(1);
        }

        if (decision->go_forward == false)
            go_back(buffer);
        else
            lexem_item->data[index++] = c;

        current_state = decision->next_state;
    } while ((c = get_next_char(buffer)) != EOF);

    if (table.final_states[current_state] == true)
        return lexem_item;

    return NULL;
}

lexem *allocate_lexem_buffer(void)
{
    lexem *buffer = (lexem *)malloc(sizeof(lexem));

    buffer->data = (char *)malloc(sizeof(char) * 200);

    return buffer;
}

void deallocate_lexem_buffer(lexem *kill)
{
    if (kill == NULL)
        return;

    if (kill->data != NULL)
        free(kill->data);

    free(kill);
}
