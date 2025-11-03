#include <string.h>

#include "lexer.h"

#define COLUMN_SIZE sizeof(char) * 20

#define LINE_SIZE 4096 * 4

typedef struct t_list_node
{
    bool final_state;
    lexer_decision **decisions;
    struct t_list_node *next_node;
} list_node;

list_node *create_or_insert_list_node(bool final_state, lexer_decision **decisions, list_node *past_node)
{
    list_node *new = (list_node *)malloc(sizeof(list_node));

    new->final_state = final_state;
    new->decisions = decisions;
    new->next_node = NULL;

    if (past_node == NULL)
        return new;

    past_node->next_node = new;

    return new;
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

lexer_table *malloc_lexer_table(int columns, int rows)
{
    lexer_table *table = (lexer_table *)malloc(sizeof(lexer_table));

    table->final_states = (bool *)malloc(sizeof(bool) * rows);

    table->data = (lexer_decision ***)malloc(sizeof(lexer_decision **) * rows);

    return table;
}

int count_columns_by_comma(char *buffer)
{
    int columns_number = 1;

    for (int i = 0; i < strlen(buffer); i++)
        if (buffer[i] == ',')
            columns_number++;

    return columns_number;
}

void set_columns_in_line(char **columns, char *buffer)
{
    int index = 0;

    char *token = strtok(buffer, ",");

    while (token != NULL)
    {
        columns[index] = (char *)malloc(COLUMN_SIZE);
        strncpy(columns[index], token, COLUMN_SIZE - 1);
        columns[index][COLUMN_SIZE - 1] = '\0'; // Ensure null-termination
        index++;
        token = strtok(NULL, ",");
    }
}

list_node *read_buffer_and_add_node(char *buffer, int total_columns, list_node *reference_node)
{
    int index = 0;

    int columns_number = total_columns - 1;

    char **parsed_strings = (char **)malloc(COLUMN_SIZE * total_columns);

    lexer_decision **decisions = (lexer_decision **)malloc(sizeof(lexer_decision *) * (columns_number));

    char *token = strtok(buffer, ",");

    while (token != NULL)
    {
        parsed_strings[index++] = parse_string(token);
        token = strtok(NULL, ",");
    }

    for (int index = 0; index < columns_number; index++)
        decisions[index] = parse_decision(parsed_strings[index]);

    bool final_state = parsed_strings[columns_number][0] == 'V' ? true : false;

    free(parsed_strings);

    return create_or_insert_list_node(final_state, decisions, reference_node);
}

void deallocate_lexem_table(lexer_table *table)
{
    for (int row = 0; row < table->rows; row++)
    {
        for (int column = 0; column < table->columns; column++)
            free(table->data[row][column]);

        free(table->data[row]);
    }

    free(table->final_states);
    free(table->data);
    free(table->characters);
    free(table);
}

lexer_table *read_lexer_table(FILE *file)
{
    char buffer[LINE_SIZE];
    int rows_number = 0, row_index = 0;

    fgets(buffer, LINE_SIZE - 1, file);

    int total_columns_number = count_columns_by_comma(buffer);

    char **columns = (char **)malloc(sizeof(char *) * total_columns_number);

    set_columns_in_line(columns, buffer);

    int columns_number = total_columns_number - 1;

    list_node *head_node = create_or_insert_list_node(-1, NULL, NULL);

    list_node *reference_node = head_node, *free_node = NULL;

    while (fgets(buffer, LINE_SIZE, file))
    {
        rows_number++;
        reference_node = read_buffer_and_add_node(buffer, total_columns_number, reference_node);
    }

    free_node = head_node;

    reference_node = head_node->next_node;

    lexer_table *table = malloc_lexer_table(columns_number, rows_number);

    while (reference_node != NULL)
    {
        free(free_node);

        table->data[row_index] = reference_node->decisions;
        table->final_states[row_index] = reference_node->final_state;

        row_index++;

        free_node = reference_node;
        reference_node = reference_node->next_node;
    }

    free(free_node);

    table->characters = columns;

    table->rows = rows_number;
    table->columns = columns_number;

    return table;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 0;

    FILE *table_file = fopen(argv[1], "rb");

    if (table_file == NULL)
        return 0;

    lexer_table *table = read_lexer_table(table_file);

    // Generate a C file that defines the lexer_table statically
    FILE *out = fopen("src/lexer_table.c", "w");
    if (!out)
    {
        fprintf(stderr, "failed to open output file.\n");
        return 1;
    }

    fprintf(out, "#include \"lexer.h\"\n\n");

    // Write characters array
    fprintf(out, "const char *lexer_table_characters[] = {");
    for (int i = 0; i < table->columns; i++)
    {
        fprintf(out, "\"%s\"%s", table->characters[i], (i < table->columns - 1) ? ", " : "");
    }
    fprintf(out, "};\n\n");

    // Write final_states array
    fprintf(out, "const bool lexer_table_final_states[] = {");
    for (int i = 0; i < table->rows; i++)
    {
        fprintf(out, "%s%s", table->final_states[i] ? "true" : "false", (i < table->rows - 1) ? ", " : "");
    }
    fprintf(out, "};\n\n");

    // Write decisions
    for (int i = 0; i < table->rows; i++)
    {
        fprintf(out, "const lexer_decision lexer_table_row_%d[] = {", i);
        for (int j = 0; j < table->columns; j++)
        {
            lexer_decision *d = table->data[i][j];
            fprintf(out, "{%d, %s}%s", d->next_state, d->go_forward ? "true" : "false", (j < table->columns - 1) ? ", " : "");
        }
        fprintf(out, "};\n");
    }
    fprintf(out, "\n");

    // Write pointers to rows
    fprintf(out, "const lexer_decision *lexer_table_data[] = {\n", table->columns);
    for (int i = 0; i < table->rows; i++)
    {
        fprintf(out, "    lexer_table_row_%d%s\n", i, (i < table->rows - 1) ? "," : "");
    }
    fprintf(out, "};\n\n");

    // Write the lexer_table struct
    fprintf(out,
            "const lexer_table LEXER_TABLE = {\n"
            "    .characters = (char **)lexer_table_characters,\n"
            "    .final_states = (bool *)lexer_table_final_states,\n"
            "    .data = (lexer_decision ***)lexer_table_data,\n"
            "    .rows = %d,\n"
            "    .columns = %d\n"
            "};\n",
            table->rows, table->columns);

    fclose(out);
    printf("static lexer_table written to src/lexer_table.c\n");

    deallocate_lexem_table(table);

    return 0;
}
