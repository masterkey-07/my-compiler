#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <ctype.h>
#include <file_buffer.h>

typedef struct
{
    int next_state;
    bool go_forward;
} lexer_decision;

typedef struct
{
    char **characters;
    int **states;
    lexer_decision ***data;
} lexer_table;

typedef struct
{
    unsigned int token;
    char *data;
} lexem;

lexem *allocate_lexem_buffer();

void deallocate_lexem_buffer(lexem *kill);

lexem get_next_lexem(file_buffer *buffer, lexer_table *table);

lexer_table *read_lexer_table(FILE *file);

#endif