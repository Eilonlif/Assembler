#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "string_operations.h"
#include "error.h"
#include "utils.h"

#define COMMENT_IDENTIFIER ';'
#define INSTRUCTION_IDENTIFIER '.'
#define END_OF_LABEL_IDENTIFIER ':'

#define MAX_LABEL_SIZE 33
#define OPERAND_NAMES_TABLE_SIZE 16

#define IC 100
#define DC 0

enum line_and_instruction_types {EMPTY_LINE, COMMENT, INSTRUCTION_LINE, COMMAND_LINE, DATA_INSTRUCTION, STRING_INSTRUCTION, ENTRY_INSTRUCTION, EXTERN_INSTRUCTION};
enum addressing_modes {IMMEDIATE, DIRECT, INDEX_ADDRESSING, REGISTER_DIRECT};
enum symbol_attributes {DATA_ATTRIBUTE, CODE_ATTRIBUTE, ENTRY_ATTRIBUTE, EXTERNAL_ATTRIBUTE};

extern char* instructions[];

typedef struct symbol {
    char symbol_name[MAX_LINE_SIZE];
    int value;
    int base_address;
    int offset;
    /* 4 is the theoretical maximum although there can never be 4 I think so just to be safe... */
    short attributes[4];
} symbol;

int has_label(char *line);

int check_valid_label(char* line);

short get_instruction_type(char *line);

int identify_line(char *line);

void create_operand_table(int tmp_table[16][6]);

short check_in_symbol_table(symbol *symbol_table, int symbol_table_size, char *symbol_name);

short check_in_operand_table(char **operand_names_table, char *operand);

int identify_addressing_modes(char* line);

short insert_to_symbol_table(symbol *symbol_table, int *symbol_table_size, char symbol_name[MAX_LINE_SIZE], int value,int base_address, int offset, short attributes[4]);

void assembler_pass_1(char *file_name);

#endif

