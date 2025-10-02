#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <ctype.h>
#include <file_buffer.h>
#include <oflex.h>

typedef struct
{
    unsigned int token;
    char *data;
} lexem;

lexem *allocate_lexem_buffer();

void deallocate_lexem_buffer(lexem *kill);

lexem *get_next_lexem(file_buffer *buffer, lexer_table table);

#endif