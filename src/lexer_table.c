#include "lexer.h"

const char *lexer_table_characters[] = {"digit", "space"};

const bool lexer_table_final_states[] = {true};

const lexer_decision lexer_table_row_0[] = {{0, true}, {-1, false}};

const lexer_decision *lexer_table_data[] = {
    lexer_table_row_0
};

const lexer_table LEXER_TABLE = {
    .characters = (char **)lexer_table_characters,
    .final_states = (bool *)lexer_table_final_states,
    .data = (lexer_decision ***)lexer_table_data,
    .rows = 1,
    .columns = 2
};
