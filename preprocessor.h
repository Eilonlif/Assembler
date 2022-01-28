#include "settings.h"
#include "string_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MACRO_START "macro"
#define MACRO_END "endm"

enum MACRO_SEARCH{MACRO_NOT_FOUND = -1};

typedef struct macro {
    char name[MAX_LINE_SIZE];
    char* code;
} macro;

int check_in_macro_table(struct macro macro_table[MAX_FILE_SIZE], int macro_table_size, char field[]);

char* read_macro(FILE *fp);

int start_preprocessor(char* file_name, char* processed_file_name);
