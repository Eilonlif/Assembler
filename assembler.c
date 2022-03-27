#include "assembler.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>

char *instructions[] = {".data", ".string", ".entry", ".extern"};

int op_table[16][6] = {
        {0,  0,  0,  3,  1,  3},
        {1,  0,  0,  3,  0,  3},
        {2,  10, 0,  3,  1,  3},
        {2,  11, 0,  3,  1,  3},
        {4,  0,  1,  2,  1,  3},
        {5,  10, -1, -1, 1,  3},
        {5,  11, -1, -1, 1,  3},
        {5,  12, -1, -1, 1,  3},
        {5,  13, -1, -1, 1,  3},
        {9,  10, -1, -1, 1,  2},
        {9,  11, -1, -1, 1,  2},
        {9,  12, -1, -1, 1,  2},
        {12, 0,  -1, -1, 1,  3},
        {13, 0,  -1, -1, 0,  3},
        {14, 0,  -1, -1, -1, -1},
        {15, 0,  -1, -1, -1, -1}};


int check_valid_label_name(char *line) {
    int i;
    char c;
    for (i = 0; i < strlen(line) - 1; i++) {
        c = line[i];
        if (!(('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))) {
            return FALSE;
        }
    }
    return TRUE;
}

int check_valid_label(char *label) {
    if (strlen(label) > MAX_LABEL_SIZE) {
        return FALSE;
    }
    return label[0] != COMMA_IDENTIFIER && check_valid_label_name(label) &&
           (label[strlen(label) - 1] == END_OF_LABEL_IDENTIFIER);
}

char *has_label(char *line) {
    int n_fields = 1;
    char *fields[n_fields];
    get_first_n_fields(line, n_fields, fields);
    if (check_valid_label(fields[0])) {
        fields[0][strlen(fields[0]) - 1] = '\0';
        return fields[0];
    }
    return "\0";
}

void clear_values(int *arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

int get_instruction_type(char *line) {
    int i;
    int f;
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
    char *fields[n_fields];
    char label[MAX_LABEL_SIZE];
    get_first_n_fields(line, n_fields, fields);
    tmp = (char *) calloc((strlen(line) + 1), sizeof(char));
    strcpy(tmp, line);
    if (strlen(trim(tmp)) == 0) {
        return EMPTY_LINE;
    }
    if (fields[0][0] == COMMENT_IDENTIFIER) {
        return COMMENT;
    }

    if (strlen(has_label(line)) != 0) {
        if (fields[1][0] == INSTRUCTION_IDENTIFIER) {
            /* its not different whether there is a label or not... */
            return get_instruction_type(fields[1]);
        }
    }

    if (fields[0][0] == INSTRUCTION_IDENTIFIER) {
        return get_instruction_type(fields[0]);
    }
    return COMMAND_LINE;
}


int check_in_symbol_table(symbol *symbol_table, int symbol_table_size, char *symbol_name) {
    int i;
    for (i = 0; i < symbol_table_size; i++) {
        if (strlen(symbol_name) > 0 && strncmp(symbol_table[i].symbol_name, symbol_name, strlen(symbol_name)) == 0) {
            return i;
        }
    }
    return -1;
}


int check_in_operand_table(char **operand_names_table, char *operand) {
    int i;
    for (i = 0; i < OPERAND_NAMES_TABLE_SIZE; i++) {
        if (strcmp(operand_names_table[i], operand) == 0) {
            return TRUE;
        }
    }
    return FALSE;

}


/*
 * copying l2 to l1
 * */
void cpy_int_lists_for_symbol(int *l1, int *l2) {
    int i;
    for (i = 0; i < 4; i++) {
        l1[i] = l2[i];
    }
}

int insert_to_symbol_table(symbol **symbol_table, int *symbol_table_size, char symbol_name[MAX_LINE_SIZE], int value,
                           int base_address, int offset, int attributes[4], int attribute_size) {
    if (check_in_symbol_table(*symbol_table, *symbol_table_size, symbol_name) != -1) {
        return SYMBOL_ALREADY_IN_SYMBOL_TABLE;
    }
    symbol s;
    memset(s.symbol_name, 0, MAX_LINE_SIZE);
    strcpy(s.symbol_name, "");
    strcpy(s.symbol_name, symbol_name);
    s.value = value;
    s.base_address = base_address;
    s.offset = offset;
    s.attributes_size = attribute_size;
    cpy_int_lists_for_symbol(s.attributes, attributes);

    (*symbol_table)[(*symbol_table_size) - 1] = s;
    *symbol_table = (symbol *) realloc(*symbol_table, (++(*symbol_table_size)) * sizeof(symbol));
    return NO_ERROR;
}


/**
 *
 * @param value
 * @return true if no spaces, false otherwise
 */
int check_for_spaces(char *value) {
    /* beware */
    int i;
    for (i = 0; i < strlen(value); i++) {
        if (isspace(value[i])) {
            return FALSE;
        }
    }
    return TRUE;
}


int check_hashtag(char *line) {
    if (line[0] == HASHTAG_IDENTIFIER) {
        return is_whole_number(line + 1);
    }
    return FALSE;
}


int check_register(char *line) {
    int num;
    if (line[0] == REGISTER_IDENTIFIER) {
        if (is_whole_number(line + 1)) {
            num = atoi(line + 1);
        } else {
            return -1;
        }
        if (0 <= num && num <= 15) {
            return num;
        }
    }
    return -1;
}


int check_register_brackets(char *line) {
    if (line[0] == REGISTER_OPEN_IDENTIFIER && line[strlen(line) - 1] == REGISTER_CLOSE_IDENTIFIER) {
        line[strlen(line) - 1] = '\0';
        return check_register(line + 1);
    }
    return -1;
}

int check_label_with_register(char *line) {
    char *token;
    char tmp_line[MAX_LINE_SIZE];
    const char s[2] = {REGISTER_OPEN_IDENTIFIER, '\0'};
    int register_number;
    strcpy(tmp_line, line);
    token = strtok(line, s);
    /* i.e: not a case of label[r_n] */
    if (strlen(trim(token)) == strlen(trim(tmp_line))) {
        return -1;
    }
    register_number = check_register_brackets(tmp_line + strlen(token));
    if (check_valid_label_name(token) && register_number != -1) {
        return register_number;
    }
    return -1;
}


/* TODO: REMEMBER, line has to be trimmed */
int split_by_comma(char *line, char **values, int label_flag, int *values_size) {
    int i;
    char *result;
    strcpy(line, clean_spaces(line));

    result = (char *) calloc(20 * MAX_LINE_SIZE, sizeof(char));
    char *token_space;
    const char s_space[2] = {SPACE_IDENTIFIER, '\0'};

    char *token_comma;
    const char s_comma[2] = {COMMA_IDENTIFIER, '\0'};

    token_space = strtok(line, s_space);

    if (label_flag) {
        token_space[strlen(token_space) - 1] = '\0';
        strncpy(&result[0], token_space, MAX_LINE_SIZE);
        token_space = strtok(NULL, s_space);
    }
    strncpy(&result[1 * MAX_LINE_SIZE], token_space, MAX_LINE_SIZE);
    i = 2;
    token_comma = strtok(NULL, s_comma);

    while (token_comma != NULL) {
        if (strlen(trim(token_comma)) == 0) {
        } else {
            strncpy(&(result[i * MAX_LINE_SIZE]), trim(token_comma), MAX_LINE_SIZE);
            i += 1;
            token_comma = strtok(NULL, s_comma);
        }
    }
    *values_size = i;
    *values = calloc(i * MAX_LINE_SIZE, sizeof(char));
    memcpy(*values, result, i * MAX_LINE_SIZE);

    free(result);
    return NO_ERROR;
}


/* I changed this function */
/* TODO: REMEMBER!! *line has to start after the opcode, otherwise all of the functions would NOT work*/
int get_values(char *line, char **values, int *values_size) {
    char label[MAX_LABEL_SIZE];
    /* TODO: check that */
    strcpy(label, has_label(line));

    /* TRUE: yes label, FALSE: no label*/

    split_by_comma(line, values, strlen(label) > 0, values_size);
    return NO_ERROR;
}


int insert_value_by_index(int row, int num_index, int num) {
    /* row += 2^index * num */
    row += pow(2, num_index) * num;
    return row;
}


void calculate_register_and_addressing_mode(char *values, int field_index, int **reg_addrss_mode, symbol *symbol_table,
                                            int symbol_table_size, char *externs, int externs_size, char *labels,
                                            int labels_size) {
    int addressing_mode;

    int check_label_register;
    int check_register_okay;

    int register_num;

    check_label_register = check_label_with_register(trim(&(values[field_index * MAX_LINE_SIZE])));
    check_register_okay = check_register(trim(&values[field_index * MAX_LINE_SIZE]));

    if (check_label_register != -1) {
        register_num = check_label_register;
    } else if (check_register_okay != -1) {
        register_num = check_register_okay;
    } else {
        register_num = 0;
    }

    int j;
    int defined_in_extern = FALSE;
    int defined_in_labels = FALSE;
    for (j = 0; j < externs_size; j++) {
        if (strcmp(trim(&values[field_index * MAX_LINE_SIZE]), &(externs[j * MAX_LABEL_SIZE])) == 0) {
            defined_in_extern = TRUE;
        }
    }
    if (check_label_register == -1) {
        for (j = 0; j < labels_size; j++) {
            if (strcmp(trim(&values[field_index * MAX_LINE_SIZE]), &(labels[j * MAX_LABEL_SIZE])) == 0) {
                defined_in_labels = TRUE;
            }
        }
    }
    if (check_hashtag(trim(&values[field_index * MAX_LINE_SIZE]))) {
        addressing_mode = IMMEDIATE;
//    } else if (defined_in_labels || defined_in_extern || check_in_symbol_table(symbol_table, symbol_table_size, trim(&values[field_index * MAX_LINE_SIZE])) != -1) {
    } else if (defined_in_labels || defined_in_extern) {
        addressing_mode = DIRECT;
        printf("everything is direct??... \n");
    } else if (check_label_register != -1) {
        addressing_mode = INDEX_ADDRESSING;
    } else if (check_register_okay != -1) {
        addressing_mode = REGISTER_DIRECT;
    } else {
        printf("something is very very wrong... \n");
        addressing_mode = 0;
    }

    (*reg_addrss_mode)[0] = register_num;
    (*reg_addrss_mode)[1] = addressing_mode;
}

void insert_rows_to_table(int *rows, int rows_index, int *table, int *table_index) {
    int i;
    for (i = 0; i < rows_index; i++) {
        printf("table index: %d\n", *table_index);
        int2bin(rows[i]);

        table[(*table_index) + i] = rows[i];
    }
    (*table_index) += rows_index;
}

/* TODO: Still need to work out the case for .data and .string */
int calculate_binary_code(char *line, char **operand_names_table, int *table, int *table_index, symbol *symbol_table,
                          int symbol_table_size, char *externs, int externs_size, char *labels, int labels_size) {
//    printf("line in calculate_binary_code: %s\n", line);

    char *values;

    int rows_index;

    int *reg_addressing_mode_list;
    reg_addressing_mode_list = (int *) malloc(sizeof(int) * 2);
    reg_addressing_mode_list[0] = 0;
    reg_addressing_mode_list[1] = 0;
    int value_after_hashtag;

    int opcode;
    int funct;

    int values_size;


    /* values: {label opcode a1, a2} */

    get_values(line, &values, &values_size);
//    int rows[values_size];
    int rows[20];
    clear_values(rows, MAX_LINES_IN_TABLE);

    int i;
    for (i = 0; i < OPERAND_NAMES_TABLE_SIZE; i++) {
        if (strcmp(&values[MAX_LINE_SIZE], operand_names_table[i]) == 0) {
            funct = op_table[i][1];
            opcode = op_table[i][0];
        }
    }

    int j;
    rows_index = 1;
    if (strcmp(&values[MAX_LINE_SIZE], instructions[0]) == 0) {
        for (j = 2; j < values_size; j++) {
            rows[j - 2] = insert_value_by_index(rows[0], A_INDEX, 1);
            int num;
            num = atoi(&values[j * MAX_LINE_SIZE]);
            if (num < 0) {
                num = (~num) + 1;
            }
            rows[j - 2] += num;
            DC++;

        }
        rows_index = values_size-2;

    } else if (strcmp(&values[MAX_LINE_SIZE], instructions[1]) == 0) {
        (&values[2 * MAX_LINE_SIZE])[strlen(&values[2 * MAX_LINE_SIZE]) - 1] = '\0';

        for (j = 0; j < strlen(&values[2 * MAX_LINE_SIZE]) - 1; j++) {
            rows[j] = insert_value_by_index(rows[0], A_INDEX, 1);
            /*sus...., its a char */
            rows[j] += values[(2 * MAX_LINE_SIZE) + 1];
            DC++;
        }
        rows_index = strlen(&values[2 * MAX_LINE_SIZE]);
        rows[rows_index - 1] = insert_value_by_index(rows[rows_index - 1], A_INDEX, 1);
    } else {
        rows[0] += insert_value_by_index(rows[0], opcode, 1);
        /* TODO: Check if the A_INDEX is correct or it could be something else*/
        rows[0] = insert_value_by_index(rows[0], A_INDEX, 1);
        if (opcode < 14) {
            rows_index++;
            rows[1] = insert_value_by_index(rows[1], FUNCT_INDEX, funct);
            rows[1] = insert_value_by_index(rows[1], A_INDEX, 1);

            if (opcode < 5 && strlen(&values[2 * MAX_LINE_SIZE]) > 0) {
                calculate_register_and_addressing_mode(values, 2, &reg_addressing_mode_list, symbol_table,
                                                       symbol_table_size, externs, externs_size, labels, labels_size);
                rows[1] = insert_value_by_index(rows[1], SOURCE_REGISTER_INDEX, reg_addressing_mode_list[0]);
                rows[1] = insert_value_by_index(rows[1], SOURCE_ADDRESSING_MODE_INDEX, reg_addressing_mode_list[1]);
                if (reg_addressing_mode_list[1] == IMMEDIATE) {
                    /* TODO: separate to function*/
                    value_after_hashtag = atoi(trim(&values[2 * MAX_LINE_SIZE]) + 1);;
                    if (value_after_hashtag < 0) {
                        value_after_hashtag = (~value_after_hashtag) + 1;
                    }
                    rows[rows_index] += value_after_hashtag;
                    rows[rows_index] = insert_value_by_index(rows[rows_index], A_INDEX, 1);
                    rows_index++;
                } else if (reg_addressing_mode_list[1] == DIRECT || reg_addressing_mode_list[1] == INDEX_ADDRESSING) {
                    rows_index += 2;
                }

            }
            if (strlen(&values[(opcode < 5 ? 3 : 2) * MAX_LINE_SIZE]) > 0) {
                calculate_register_and_addressing_mode(values, (opcode < 5 ? 3 : 2), &reg_addressing_mode_list,
                                                       symbol_table, symbol_table_size, externs, externs_size, labels,
                                                       labels_size);
                rows[1] = insert_value_by_index(rows[1], TARGET_REGISTER_INDEX, reg_addressing_mode_list[0]);
                rows[1] = insert_value_by_index(rows[1], TARGET_ADDRESSING_MODE_INDEX, reg_addressing_mode_list[1]);

                if (reg_addressing_mode_list[1] == IMMEDIATE) {
                    value_after_hashtag = atoi(trim(&values[(opcode < 5 ? 3 : 2) * MAX_LINE_SIZE]) + 1);;
                    if (value_after_hashtag < 0) {
                        value_after_hashtag = (~value_after_hashtag) + 1;
                    }
                    rows[rows_index] += value_after_hashtag;
                    rows[rows_index] = insert_value_by_index(rows[rows_index], A_INDEX, 1);
                    rows_index++;
                } else if (reg_addressing_mode_list[1] == DIRECT || reg_addressing_mode_list[1] == INDEX_ADDRESSING) {
                    rows_index += 2;
                }
            }
        }
    }
    printf("line: %s\n", line);
    insert_rows_to_table(rows, rows_index, table, table_index);

    //    free(&values);
    return rows_index;
}


/* TODO: externs size might be wrong last time i checked... */
void
get_externs_and_entries_and_labels(char *file_name, char **externs, char **entries, int *externs_size, char **labels,
                                   int *labels_size) {
    char *token;
    int externs_index;
    int entries_index;
    int labels_index;

    int line_number;
    const char s[2] = {' ', '\0'};
    char line[MAX_LINE_SIZE];

    int n_fields = 1;
    char *fields[n_fields];

    (*externs_size) = 1;
    (*labels_size) = 1;

    line_number = 0;

    labels_index = 0;
    entries_index = 0;
    externs_index = 0;
    /* TODO: CHECK... */
//    const char entry_name[6] = ENTRY_IDENTIFIER;
//    const char extern_name[7] = EXTERN_IDENTIFIER;
    FILE *fp = fopen(file_name, "r");
    while ((fgets(line, MAX_LINE_SIZE, fp)) != NULL) {
        strcpy(line, clean_spaces(line));
        if (strlen(trim(line)) > 0) {
            get_first_n_fields(line, n_fields, fields);
            if (fields[0][strlen(fields[0]) - 1] == END_OF_LABEL_IDENTIFIER) {
                fields[0][strlen(fields[0]) - 1] = '\0';
                strcpy(&((*labels)[labels_index * MAX_LABEL_SIZE]), trim(fields[0]));
                (*labels) = realloc(*labels, (++labels_index) * MAX_LABEL_SIZE * sizeof(char));
                (*labels_size)++;
            } else if (strstr(line, ENTRY_IDENTIFIER) != NULL) {
                /* TODO: BRUH - --- -- -- check the logic hereeeeee, s needs to be something like sapce or ,     ....*/
                token = strtok(line, s);
                token = strtok(NULL, s);
                if (check_for_spaces(trim(token))) {
                    strcpy(&((*entries)[entries_index * MAX_LABEL_SIZE]), trim(token));
                    (*entries) = realloc(*entries, (++entries_index) * MAX_LABEL_SIZE * sizeof(char));
                } else {
                    error_handler(SPACE_IN_WORD, line_number);
                }
            } else if (strstr(line, EXTERN_IDENTIFIER) != NULL) {
                /* TODO: BRUH*/
                token = strtok(line, s);
                if (line[0] == INSTRUCTION_IDENTIFIER) {
                    token = strtok(NULL, s);
                    strcpy(&((*externs)[externs_index * 33]), trim(token));
                    (*externs) = realloc(*externs, (++externs_index) * MAX_LABEL_SIZE * sizeof(char));
                    (*externs_size)++;
                } else if (check_for_spaces(trim(token))) {
                    token = strtok(NULL, s);
                    token = strtok(NULL, s);
                    strcpy(&((*externs)[externs_index * MAX_LABEL_SIZE]), trim(token));
                    (*externs) = realloc(*externs, (++externs_index) * MAX_LABEL_SIZE * sizeof(char));
                    (*externs_size)++;
                } else {
                    error_handler(SPACE_IN_WORD, line_number);
                }
                line_number++;
            }
        }
    }
    fclose(fp);
}


void output_extern (char *file_name, char *symbol_name, int current_IC) {
    char str[MAX_LINE_SIZE];
    char extern_file_name[MAX_LINE_SIZE];

    strcpy(extern_file_name, file_name);

    extern_file_name[strlen(extern_file_name) - 4] = '\0';
    strcat(extern_file_name, ".ext");

    FILE *fp = fopen(extern_file_name, "a+");
    sprintf(str, "%s BASE %d\n%s OFFSET %d\n", symbol_name, current_IC, symbol_name, current_IC + 1);
    fputs(str, fp);
    fclose(fp);


}


void check_label_or_label_register(char *parm, int **table, symbol *symbol_table, int symbol_table_size, int line_index,
                                   int *table_index_prefixes[MAX_TABLE_SIZE], char *file_name) {
    int symbol_index;
    int index;
    symbol_index = check_in_symbol_table(symbol_table, symbol_table_size, parm);
    /* TODO: valid_label_name checks until len-1*/
    int label_reg = -1;
    if ((symbol_index != -1 && check_valid_label_name(parm)) || (label_reg = check_label_with_register(parm)) != -1) {
        if (label_reg != -1) {
            char *token;
            char tmp_line[MAX_LINE_SIZE];
            const char s[2] = {REGISTER_OPEN_IDENTIFIER, '\0'};
            strcpy(tmp_line, parm);
            token = strtok(tmp_line, s);
            symbol_index = check_in_symbol_table(symbol_table, symbol_table_size, token);
        }

        index = (((*table_index_prefixes)[line_index]) - 100) + 2;
        (*table)[index] += symbol_table[symbol_index].base_address;
        (*table)[index] = insert_value_by_index((*table)[index], R_INDEX, 1);
        (*table)[index + 1] += symbol_table[symbol_index].offset;
        (*table)[index + 1] = insert_value_by_index((*table)[index + 1], R_INDEX, 1);

        if (symbol_table[symbol_index].attributes[0] == EXTERNAL_ATTRIBUTE) {
            output_extern(file_name, parm, 100 + index);
        }
        printf("%s\n", parm);
        printf("%d\n", index);
        int2bin((*table)[index]);
        int2bin((*table)[index + 1]);
    }
}


