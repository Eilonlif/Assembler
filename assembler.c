#include "assembler.h"
#include "utils.h"

char* instructions[] = {".data", ".string", ".entry", ".extern"};

int has_label(char *line) {
    /* TODO (Liraz): change function implementation to return the name of the symbol and NULL otherwise */
    /* TODO (Liraz): change any future use of the function... */
    int n_fields = 1;
    char* fields[n_fields];
    get_first_n_fields(line, n_fields, fields);
    /* TODO: free fields[0] */
    return fields[0][strlen(fields[0]) - 1] == END_OF_LABEL_IDENTIFIER;
}

/* TODO (Eilon): add checkv for opcode not a saed operand! */

int check_valid_label(char* line) {
    int i;
    char c;
    if (strlen(line) > MAX_LABEL_SIZE) {
        return FALSE;
    }
    for (i = 0; i < strlen(line) - 1; i++) {
        c = line[i];
        if (!(('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))) {
            return FALSE;
        }
    }
    return line[strlen(line) - 1] == END_OF_LABEL_IDENTIFIER;
}

short get_instruction_type(char *line) {
    int i;
    int n_fields = 1;
    char* fields[n_fields];
    get_first_n_fields(line, n_fields, fields);
    /* TODO (Eilon): Magic const */
    for (i = 0; i < 4; i++) {
        if (strcmp(line, instructions[i]) == 0) {
            switch (i) {
                case 0:
                    return DATA_INSTRUCTION;
                case 1:
                    return STRING_INSTRUCTION;
                case 2:
                    return ENTRY_INSTRUCTION;
                case 3:
                    return EXTERN_INSTRUCTION;
            }
        }
    }
    return UNDEFINED_INSTRUCTION;
}

int identify_line(char *line) {
    char *tmp;
    int n_fields = 2;
    char* fields[n_fields];
    get_first_n_fields(line, n_fields, fields);
    tmp = (char*) malloc((strlen(line) + 1) * sizeof(char));
    strcpy(tmp, line);
    if (strlen(trim(tmp)) == 0) {
        return EMPTY_LINE;
    }
    if (fields[0][0] == COMMENT_IDENTIFIER) {
        return COMMENT;
    }

    /* TODO (Eilon): logic kinda sus not gonna lie... */
    if (has_label(line)) {
       if (fields[1][0] == INSTRUCTION_IDENTIFIER) {
           return get_instruction_type(line);
       }
    }

    if (fields[0][0] == INSTRUCTION_IDENTIFIER) {
        return get_instruction_type(line);
    }
    return COMMAND_LINE;
}



void create_operand_table(int tmp_table[16][6]) {
    int i
    int j;
    int table[16][6] ={{0, -1, 0, 3, 1, 3},
                {1, -1, 0, 3, 0, 3},
                {2, 10, 0, 3, 1, 3},
                {2, 11, 0 ,3, 1, 3},
                {4, -1, 1, 2 ,1 ,3},
                {5, 10, -1, -1, 1, 3},
                {5, 11, -1, -1, 1, 3},
                {5, 12, -1, -1, 1, 3},
                {5, 13, -1, -1, 1, 3},
                {9, 10, -1, -1, 1, 2},
                {9, 11, -1, -1, 1, 2},
                {9, 12, -1, -1, 1, 2},
                {12, -1, -1, -1, 1, 3},
                {13, -1, -1, -1, 0, 3},
                {14, -1, -1, -1, -1, -1},
                {15, -1, -1, -1, -1, -1}};
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 6; j++) {
            tmp_table[i][j] = table[i][j];
        }
    }
}

short check_in_symbol_table(symbol *symbol_table, int symbol_table_size, char *symbol_name) {
    int i;
    for (i = 0; i < symbol_table_size; i++) {
        if (strcmp(symbol_table[i].symbol_name, symbol_name) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

short check_in_operand_table(char **operand_names_table, char *operand) {
    int i;
    for (i = 0; i < OPERAND_NAMES_TABLE_SIZE; i++) {
        if (strcmp(operand_names_table[i], operand) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

int identify_addressing_modes(char* line) {

}

short insert_to_symbol_table(symbol *symbol_table, int *symbol_table_size, char symbol_name[MAX_LINE_SIZE], int value, int base_address, int offset, short attributes[4]) {
    if (check_in_symbol_table(symbol_table, *symbol_table_size, symbol_name)) {
        return SYMBOL_ALREADY_IN_SYMBOL_TABLE;
    }
    symbol s;
    strcpy(s.symbol_name, symbol_name);
    s.value = value;
    s.base_address = base_address;
    s.offset = offset;
    s.attributes = attributes;
    symbol_table[*symbol_table_size] = s;
    symbol_table = (symbol*) realloc((++(*symbol_table_size)) * sizeof(symbol));
    return NO_ERROR;
}

/* TODO (Liraz): make new files for each assembler pass */
void assembler_pass_1(char *file_name) {
    char line[MAX_LINE_SIZE];
    FILE *fp = fopen(file_name, "r");
    short symbol_def_flag;
    short line_identification;
    int symbol_table_size = 0;

    char *operand_names_table[] = {"mov", "cmp", "add", "sub", "lea", "clr", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    int n_fields = 2;
    char* fields[n_fields];

    int line_number = 1;

    symbol_table_size = 1;
    symbol *symbol_table;

    symbol_table = malloc(sizeof(symbol));

    symbol_def_flag = FALSE;
    while (fgets(line, MAX_LINE_SIZE, fp)) {
        /* step 1 if has label */
        if (has_label(line)) {
            symbol_def_flag = TRUE;
        }
        line_identification = identify_line(line);  /* line identification */
        if (line_identification == UNDEFINED_INSTRUCTION) {
            error_handler(UNDEFINED_INSTRUCTION)
        }
        /* TODO (Eilon): add else?? */
        get_first_n_fields(line, n_fields, fields); /* getting first n fields */
        if (line_identification == DATA_INSTRUCTION || line_identification == STRING_INSTRUCTION) {
            if (symbol_def_flag) {
                /* TODO (Liraz): Understand the use of inserting to the symbol table*/
                /* TODO (Eilon): divide into 2 lines...*/
                error_handler(insert_to_symbol_table(symbol_table, &symbol_table_size, fields[1], /* value */ ,/* base */, /* offset */, DATA_ATTRIBUTE), line_number);
                /* TODO (Eilon): Still need to do step 7... */
                /* here go back to step 2... */
            }
        } else if (line_identification == ENTRY_INSTRUCTION || line_identification == EXTERN_INSTRUCTION) { /* step 8 */
            if (line_identification == ENTRY_INSTRUCTION) {
                /* here go back to step 2... */
            } else {
                /* TODO (Eilon): indexing could be wrong... because idk what's with the label*/
                /* TODO (Eilon): divide into 2 lines...*/
                error_handler(insert_to_symbol_table(symbol_table, &symbol_table_size, fields[1],/* value */ ,/* base */, /* offset */, EXTERNAL_ATTRIBUTE), line_number);
                /* here go back to step 2... */
            }
        } else {
            debug("debug msg");
            if (symbol_def_flag) {
                if (!check_in_operand_table(operand_names_table, fields[1])) {
                    error_handler(UNDEFINED_OPERAND, line_number);
                } else if (!check_valid_label(fields[0])) {
                    /* TODO (Eilon): do you need to add check if in symbol table? */
                    error_handler(INVALID_LABEL, line_number);
                }
                /* TODO (Eilon): else?? */
                error_handler(insert_to_symbol_table(symbol_table, &symbol_table_size, fields[2], /* value */ ,/* base */, /* offset */, CODE_ATTRIBUTE), line_number);
            }
        }





        line_number++;
    }
}
