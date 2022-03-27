#ifndef MMN14_OUTPUT_H
#define MMN14_OUTPUT_H

#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void object_output(char *file_name, int* table, int table_size);

void entries_output (char *file_name, symbol *symbol_table, int symbol_table_size);
#endif
