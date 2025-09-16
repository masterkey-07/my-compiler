#include "lexer.h"
#include <string.h>

#define COLUMN_SIZE sizeof(char) * 20
#define LINE_SIZE 4096 * 4

lexem *allocate_lexem_buffer(void)
{
    return (lexem *)malloc(sizeof(lexem));
}

void deallocate_lexem_buffer(lexem *kill)
{
    if (kill == NULL)
        return;

    if (kill->data != NULL)
        free(kill->data);

    free(kill);
}

lexem get_next_lexem(file_buffer *buffer, lexer_table *table)
{
    lexem a = {
        data : "ABC",
        token : 10
    };

    return a;
}

char *parse_string(char *string)
{
    int len = strlen(string);

    int string_index = 0;
    bool finished_elimination = false;

    for (int index = len - 1; index >= 0; index++)
        if (string[index] == '\n')
            string[index] = '\0';
        else if (string[index] == ' ')
            string[index] = '\0';
        else
            break;

    for (int index = 0; index < len; index++)
        if (string[index] == '\0')
            return string;

        else if (string[index] != ' ' || finished_elimination == true)
        {
            string[string_index++] = string[index];

            if (index != string_index - 1)
                string[index] = '\0';

            finished_elimination = true;
        }

    return string;
}

lexer_decision *create_null_decision()
{
    lexer_decision *decision = (lexer_decision *)malloc(sizeof(lexer_decision));

    decision->go_forward = false;
    decision->next_state = -1;

    return decision;
}

lexer_decision *parse_decision(char *raw_decision)
{
    if (strcmp(raw_decision, "NULL") == 0)
        return create_null_decision();

    lexer_decision *decision = (lexer_decision *)malloc(sizeof(lexer_decision));

    char *next_state = strtok(raw_decision, " ");
    char *go_forward = strtok(NULL, " ");

    decision->next_state = atoi(next_state);
    decision->go_forward = (strcmp(go_forward, "V") == 0);

    return decision;
}

lexer_table *
read_lexer_table(FILE *file)
{
    char data[LINE_SIZE];

    fgets(data, LINE_SIZE - 1, file);

    int count = 1;

    for (int i = 0; i < strlen(data); i++)
        if (data[i] == ',')
            count++;

    char *token2 = strtok(data, ",");

    char **columns = (char **)malloc(COLUMN_SIZE * count);

    columns[0] = token2;

    int i = 0;

    while (columns[i] != NULL)
        columns[++i] = strtok(NULL, ",");

    char **helper = (char **)malloc(COLUMN_SIZE * count);

    while (fgets(data, LINE_SIZE, file))
    {
        char *token = strtok(data, ",");
        i = 0;

        while (token != NULL)
        {
            helper[i++] = parse_string(token);
            token = strtok(NULL, ",");
        }

        for (int i = 0; i < count; i++)
        {
            lexer_decision *decision = parse_decision(helper[i]);
            printf("%d %d\n", decision->next_state, decision->go_forward);
        }
    }

    return NULL;
}
