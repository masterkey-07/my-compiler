#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "symbol.h"

void analyze_semantic(SymbolNode *tree, SymbolTable *table, ScopeStack *stack);

#endif