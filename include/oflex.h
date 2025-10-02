#include <stdbool.h>
#include <ctype.h>
#include <file_buffer.h>

#ifndef OFLEX_H
#define OFLEX_H

typedef struct
{
    int next_state;
    bool go_forward;
} lexer_decision;

typedef struct t_lexer_table
{
    char **characters;
    bool *final_states;
    lexer_decision ***data;
    int rows;
    int columns;
} lexer_table;

lexer_table *read_lexer_table(FILE *file);

char *create_lexer_table_buffer(lexer_table);

void deallocate_lexem_table(lexer_table *table);

#endif
