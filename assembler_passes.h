#ifndef MMN14_ASSEMBLER_PASSES_H
#define MMN14_ASSEMBLER_PASSES_H
#include "assembler.h"
#include "output.h"
void assembler_pass_1(char *file_name, int **table, int *table_index_prefixes[MAX_TABLE_SIZE], symbol **symbol_table, int *symbol_table_size);

void assembler_pass_2(char *file_name, int **table, int *table_index_prefixes[MAX_TABLE_SIZE], symbol **symbol_table, int *symbol_table_size);

void assembler(char *file_name);

#endif
