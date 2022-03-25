#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_operations.h"
#include "error.h"
#include "utils.h"

#define SPACE_IDENTIFIER ' '
#define COMMENT_IDENTIFIER ';'
#define INSTRUCTION_IDENTIFIER '.'
#define END_OF_LABEL_IDENTIFIER ':'
#define HASHTAG_IDENTIFIER '#'
#define REGISTER_IDENTIFIER 'r'
#define REGISTER_OPEN_IDENTIFIER '['
#define REGISTER_CLOSE_IDENTIFIER ']'

#define ENTRY_IDENTIFIER ".entry"
#define EXTERN_IDENTIFIER ".extern"

#define MAX_TABLE_SIZE 8192

#define MAX_LABEL_SIZE 33
#define MAX_OPCODE_SIZE 4
#define OPERAND_NAMES_TABLE_SIZE 16

/* TODO: NOT CORRECT,  What about .string "abcde\0"*/
#define MAX_LINES_IN_TABLE 5

#define A_INDEX 18
#define R_INDEX 17
#define E_INDEX 16
#define FUNCT_INDEX 12
#define SOURCE_REGISTER_INDEX 8
#define SOURCE_ADDRESSING_MODE_INDEX 6
#define TARGET_REGISTER_INDEX 2
#define TARGET_ADDRESSING_MODE_INDEX 0


int IC;
int DC;

enum line_and_instruction_types {EMPTY_LINE, COMMENT, INSTRUCTION_LINE, COMMAND_LINE, DATA_INSTRUCTION, STRING_INSTRUCTION, ENTRY_INSTRUCTION, EXTERN_INSTRUCTION};
enum addressing_modes {IMMEDIATE, DIRECT, INDEX_ADDRESSING, REGISTER_DIRECT};
enum symbol_attributes {DATA_ATTRIBUTE, CODE_ATTRIBUTE, ENTRY_ATTRIBUTE, EXTERNAL_ATTRIBUTE};

extern char* instructions[];

typedef struct symbol {
    char symbol_name[MAX_LINE_SIZE];
    int value;
    int base_address;
    int offset;
    int attributes_index;
    /* 4 is the theoretical maximum although there can never be 4 I think so just to be safe... */
    int attributes[4];
    int attributes_size;
} symbol;

/* formula: (2^(index+1) * number) */


/*
typedef struct values {
    char* v;
    values next;
} values;
*/


typedef struct data_line {
    char label[MAX_LABEL_SIZE];
    char opcode[MAX_OPCODE_SIZE];
    /* TODO: MAGIC NUMBER... */
    char values[3][MAX_LINE_SIZE];
} data_line;

int check_valid_label_name(char* line);

int check_valid_label(char* label);

char* has_label(char *line);

int get_instruction_type(char *line);

int identify_line(char *line);

void create_operand_table(int tmp_table[16][6]);

int check_in_symbol_table(symbol *symbol_table, int symbol_table_size, char *symbol_name);

int check_in_operand_table(char **operand_names_table, char *operand);

int insert_to_symbol_table(symbol *symbol_table, int *symbol_table_size, char symbol_name[MAX_LINE_SIZE], int value, int base_address, int offset, int attributes[4], int attributes_size);

int check_for_spaces(char* value);

int check_hashtag(char* line);

int check_register(char* line);

int check_register_brackets(char* line);

int check_label_with_register(char* line);

int split_by_comma(char* line, char** values, int label_flag);

int get_values(char *line, char**values);

int insert_value_by_index(int row, int num_index, int num);

void calculate_register_and_addressing_mode(char **values, int field_index, int** reg_addrss_mode);

void insert_rows_to_table(int* rows, int rows_index, int* table, int *table_index);

int calculate_binary_code(char *line, char **operand_names_table, int* table, int *table_index);

void clear_values(int *arr, int size);

void get_externs_and_entries(char *file_name, char **externs, char **entries);

void clear_symbol_for_symbol_table(symbol s);

void cpy_int_lists_for_symbol(int* l1, int *l2);

void check_label_or_label_register(char *parm, int** table, symbol** symbol_table, int symbol_table_size, int line_index, int *table_index_prefixes[MAX_TABLE_SIZE]);

#endif

