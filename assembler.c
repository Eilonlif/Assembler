#include "assembler.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>

char *instructions[] = {".data", ".string", ".entry", ".extern"};

int op_table[16][6] = {
        {0,  -1, 0,  3,  1,  3},
        {1,  -1, 0,  3,  0,  3},
        {2,  10, 0,  3,  1,  3},
        {2,  11, 0,  3,  1,  3},
        {4,  -1, 1,  2,  1,  3},
        {5,  10, -1, -1, 1,  3},
        {5,  11, -1, -1, 1,  3},
        {5,  12, -1, -1, 1,  3},
        {5,  13, -1, -1, 1,  3},
        {9,  10, -1, -1, 1,  2},
        {9,  11, -1, -1, 1,  2},
        {9,  12, -1, -1, 1,  2},
        {12, -1, -1, -1, 1,  3},
        {13, -1, -1, -1, 0,  3},
        {14, -1, -1, -1, -1, -1},
        {15, -1, -1, -1, -1, -1}};


/* TODO (Eilon): add check for opcode not a saed operand! */

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
    /*TODO: check the comma identifier...., I think it's good......... */
    return label[0] != COMMA_IDENTIFIER && check_valid_label_name(label) &&
           (label[strlen(label) - 1] == END_OF_LABEL_IDENTIFIER);
}

char *has_label(char *line) {
    int n_fields = 1;
    char *fields[n_fields];
    get_first_n_fields(line, n_fields, fields);
    /* TODO: free fields[0] */
    if (check_valid_label(fields[0])) {
        /* TODO (Eilon & Liraz): check that... */
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
    printf("in get_instruction_type\n");
    int i;
    int f;
    /* TODO (Eilon): Magic const */
    for (i = 0; i < 4; i++) {
        f = strcmp(line, instructions[i]) == 0;
        printf("string 1, 2, comp: %s, %s, %d\n", line, instructions[i], strcmp(line, instructions[i]) == 0);
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
    tmp = (char *) malloc((strlen(line) + 1) * sizeof(char));
    strcpy(tmp, line);
    if (strlen(trim(tmp)) == 0) {
        return EMPTY_LINE;
    }
    if (fields[0][0] == COMMENT_IDENTIFIER) {
        return COMMENT;
    }

    /* TODO (Eilon): logic kinda sus not gonna lie... */
/*    strcpy(label, *has_label(line)) */
/* TODO: check the *has_label........ */
    if (strlen(has_label(line)) != 0) {
        if (fields[1][0] == INSTRUCTION_IDENTIFIER) {
            /* its not different whether thers a label or not... */
            return get_instruction_type(fields[1]);
        }
    }

    if (fields[0][0] == INSTRUCTION_IDENTIFIER) {
        return get_instruction_type(fields[0]);
    }
    return COMMAND_LINE;
}


void create_operand_table(int tmp_table[16][6]) {
    int i;
    int j;
    int table[16][6] = {
            {0,  -1, 0,  3,  1,  3},
            {1,  -1, 0,  3,  0,  3},
            {2,  10, 0,  3,  1,  3},
            {2,  11, 0,  3,  1,  3},
            {4,  -1, 1,  2,  1,  3},
            {5,  10, -1, -1, 1,  3},
            {5,  11, -1, -1, 1,  3},
            {5,  12, -1, -1, 1,  3},
            {5,  13, -1, -1, 1,  3},
            {9,  10, -1, -1, 1,  2},
            {9,  11, -1, -1, 1,  2},
            {9,  12, -1, -1, 1,  2},
            {12, -1, -1, -1, 1,  3},
            {13, -1, -1, -1, 0,  3},
            {14, -1, -1, -1, -1, -1},
            {15, -1, -1, -1, -1, -1}};
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 6; j++) {
            tmp_table[i][j] = table[i][j];
        }
    }
}


int check_in_symbol_table(symbol *symbol_table, int symbol_table_size, char *symbol_name) {
    printf("in check_in_symbol_table\n");
    int i;
    for (i = 0; i < symbol_table_size; i++) {
        printf("name: '%s'\n", symbol_table[i].symbol_name);
//        printf("leng: %d\n", strlen(symbol_table[i].symbol_name));
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

void clear_symbol_for_symbol_table(symbol *s) {
    s->value = 0;
    s->base_address = 0;
    s->offset = 0;
}

/*
 * copying l2 to l1
 * */
void cpy_int_lists_for_symbol(int *l1, int *l2) {
    int i;
    /* TODO: remove magic number for max num of attributes in a symbol or something*/
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

    *symbol_table[(*symbol_table_size) - 1] = s;
    printf("symbol_table_size: %d\n", *symbol_table_size);
    *symbol_table = (symbol *) realloc(*symbol_table, (++(*symbol_table_size)) * sizeof(symbol));
//    clear_symbol_for_symbol_table(symbol_table[*symbol_table_size - 1]);
    return NO_ERROR;
}

/* TODO: remember to use this function in the right spots, we didnt do that in the meeting*/
/* TODO: REMEMBER, value has to be trimmed */
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


/* TODO: REMEMBER, line has to be trimmed */
int check_hashtag(char *line) {
    /* beware */
    if (line[0] == HASHTAG_IDENTIFIER) {
        /* TODO: check that, fishy... maybe sizeof(char) */

        return is_whole_number(line + 1);
    }
    return FALSE;
}


/* TODO: REMEMBER, line has to be trimmed */
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


/* TODO: REMEMBER, line has to be trimmed */
int check_register_brackets(char *line) {
    if (line[0] == REGISTER_OPEN_IDENTIFIER && line[strlen(line) - 1] == REGISTER_CLOSE_IDENTIFIER) {
        return check_register(line);
    }
    return -1;
}


/* TODO: REMEMBER, line has to be trimmed */
int check_label_with_register(char *line) {
    char *token;
    const char s[2] = {REGISTER_OPEN_IDENTIFIER, '\0'};
    int register_number;
    token = strtok(line, s);
    /* i.e: not a case of label[r_n] */
    if (strlen(token) == strlen(line)) {
        return -1;
    }
    register_number = check_register_brackets(line + strlen(token) - 1);
    if (check_valid_label_name(token) && register_number != -1) {
        return register_number;
    }
    return -1;
}


/* TODO: REMEMBER, line has to be trimmed */
int split_by_comma(char *line, char **values, int label_flag) {
    int i;
    char *result;

    /* beware */

    /* TODO: YOU DUMBASS, first split by space after Liraz's function, and then split by ','*/
    /* TODO: Also, this function could replace a lot of unnecessary code (i think) */
    strcpy(line, clean_spaces(line));
    result = (char*)malloc((label_flag + 1) * MAX_LINE_SIZE * sizeof(char));
    char *token_space;
    const char s_space[2] = {SPACE_IDENTIFIER, '\0'};

    char *token_comma;
    const char s_comma[2] = {COMMA_IDENTIFIER, '\0'};

    token_space = strtok(line, s_space);

    if (label_flag) {
        strcpy(&result[0], token_space);
        token_space = strtok(NULL, s_space);
    }
    strcpy(&result[1*MAX_LINE_SIZE], token_space);
    int j;
//    for (j = 0; j < OPERAND_NAMES_TABLE_SIZE; j++) {
//
//    }

    /* this is for leaving an empty slot if there's no label */
    /* TODO: check that */
    i = 2;
    token_comma = trim(strtok(NULL, s_comma));
    while (token_comma != NULL) {
//        printf("trying result not reallocdesdd: %s\n", &result[MAX_LINE_SIZE * (i + 1)]);
        result = realloc(result, (i + 1) * MAX_LINE_SIZE * sizeof(char));
        printf("trimmed token: '%s'\n", trim(token_comma));
        strcpy(&result[MAX_LINE_SIZE * (i)], trim(token_comma));
        printf("result:::::\n");
        printf("result: %s\n", &result[MAX_LINE_SIZE * i]);
        i += 1;
        token_comma = strtok(NULL, s_comma);
    }
    *values = result;
//    int k;
//
//    for (k = 0; k < i; k++) {
//        printf("&result[k * MAX_LINE_SIZE]: %s\n", &result[k * MAX_LINE_SIZE]);
//        free(&result[k * MAX_LINE_SIZE]);
//        printf("after free...\n");
//    }
    free(result);
    return NO_ERROR;
}


/* I changed this function */
/* TODO: REMEMBER!! *line has to start after the opcode, otherwise all of the functions would NOT work*/
int get_values(char *line, char **values) {
    char label[MAX_LABEL_SIZE];
    /* TODO: check that */
    strcpy(label, has_label(line));

    /* TRUE: yes label, FALSE: no label*/
    split_by_comma(line, values, strlen(label) > 0);
    return NO_ERROR;
}


int insert_value_by_index(int row, int num_index, int num) {
    /* row += 2^index * num */
    row += pow(2, num_index) * num;
    return row;
}


void calculate_register_and_addressing_mode(char *values, int field_index, int **reg_addrss_mode) {
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

    if (check_hashtag(trim(&values[field_index * MAX_LINE_SIZE]))) {
        addressing_mode = IMMEDIATE;
    } else if (check_valid_label(trim(&values[field_index * MAX_LINE_SIZE]))) {
        addressing_mode = DIRECT;
    } else if (check_label_register != -1) {
        addressing_mode = INDEX_ADDRESSING;
    } else if (check_register_okay != -1) {
        addressing_mode = REGISTER_DIRECT;
    } else {
        addressing_mode = 0;
    }

    (*reg_addrss_mode)[0] = register_num;
    (*reg_addrss_mode)[1] = addressing_mode;
}

void insert_rows_to_table(int *rows, int rows_index, int *table, int *table_index) {
    int i;
    for (i = 0; i < rows_index; i++) {
        table[(*table_index) + i] = rows[i];
    }
    (*table_index) += rows_index;
}

/* TODO: Still need to work out the case for .data and .string */
int calculate_binary_code(char *line, char **operand_names_table, int *table, int *table_index) {
    printf("line in calculate_binary_code: %s\n", line);

    char *values;

    int rows[MAX_LINES_IN_TABLE];
    int rows_index;

    int *reg_addressing_mode_list;
    reg_addressing_mode_list = (int *) malloc(sizeof(int) * 2);
    reg_addressing_mode_list[0] = 0;
    reg_addressing_mode_list[1] = 0;
    int value_after_hashtag;

    int opcode;
    int funct;

    clear_values(rows, MAX_LINES_IN_TABLE);
    rows_index = 0;
    /* values: {label opcode a1, a2} */

    get_values(line, &values);
    int i;
    for (i = 0; i < OPERAND_NAMES_TABLE_SIZE; i++) {
        if (strcmp(&values[MAX_LINE_SIZE], operand_names_table[i]) == 0) {
            opcode = op_table[i][0];
            if (opcode != -1) { /* ermmmmmm (u changed the opcode from something, idk) mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/
                funct = opcode;
            }
        }
    }

    rows[0] += opcode;
    /* TODO: Check if the A_INDEX is correct or it could be something else*/
    rows[0] = insert_value_by_index(rows[0], A_INDEX, 1);

    rows[1] = insert_value_by_index(rows[1], FUNCT_INDEX, funct);
    rows[1] = insert_value_by_index(rows[1], A_INDEX, 1);

    if (strlen(&values[2 * MAX_LINE_SIZE] ) > 0) {
        calculate_register_and_addressing_mode(values, 2, &reg_addressing_mode_list);
        rows[1] = insert_value_by_index(rows[1], SOURCE_REGISTER_INDEX, reg_addressing_mode_list[0]);
        rows[1] = insert_value_by_index(rows[1], SOURCE_ADDRESSING_MODE_INDEX, reg_addressing_mode_list[1]);
        if (reg_addressing_mode_list[1] == IMMEDIATE) {
            /* TODO: separate to function*/
            value_after_hashtag = atoi(trim(&values[2 * MAX_LINE_SIZE]) + 1);
            value_after_hashtag = (~value_after_hashtag) + 1;
            rows[rows_index] = value_after_hashtag;
            rows[rows_index] = insert_value_by_index(rows[rows_index], A_INDEX, 1);
            rows_index++;
        } else if (reg_addressing_mode_list[1] == DIRECT || reg_addressing_mode_list[1] == INDEX_ADDRESSING) {
            rows_index += 2;
        }
    }
    if (strlen(&values[3 * MAX_LINE_SIZE] ) > 0) {
        calculate_register_and_addressing_mode(values, 3, &reg_addressing_mode_list);
        rows[1] = insert_value_by_index(rows[1], TARGET_REGISTER_INDEX, reg_addressing_mode_list[0]);
        rows[1] = insert_value_by_index(rows[1], TARGET_ADDRESSING_MODE_INDEX, reg_addressing_mode_list[1]);

        if (reg_addressing_mode_list[1] == IMMEDIATE) {
            value_after_hashtag = atoi(trim(&values[3 * MAX_LINE_SIZE]) + 1);
            value_after_hashtag = (~value_after_hashtag) + 1;
            rows[rows_index] = value_after_hashtag;
            rows[rows_index] = insert_value_by_index(rows[rows_index], A_INDEX, 1);
            rows_index++;
        } else if (reg_addressing_mode_list[1] == DIRECT || reg_addressing_mode_list[1] == INDEX_ADDRESSING) {
            rows_index += 2;
        }
    }

    insert_rows_to_table(rows, rows_index, table, table_index);
//    free(&values);
    return rows_index;
}


void get_externs_and_entries(char *file_name, char **externs, char **entries) {
    char *token;
    int externs_index;
    int entries_index;
    int line_number;
    const char s[2] = {',', '\0'};
    char line[MAX_LINE_SIZE];
    line_number = 0;
    entries_index = 0;
    externs_index = 0;
    /* TODO: CHECK... */
    const char entry_name[6] = ENTRY_IDENTIFIER;
    const char extern_name[7] = EXTERN_IDENTIFIER;
    FILE *fp = fopen(file_name, "r");
    while ((fgets(line, MAX_LINE_SIZE, fp)) != NULL) {
        if (strstr(line, ENTRY_IDENTIFIER) != NULL) {
            /* TODO: BRUH - --- -- -- check the logic hereeeeee, s needs to be something like sapce or ,     ....*/
            token = strtok(line, s);
            token = strtok(NULL, s);
            if (check_for_spaces(trim(token))) {
                strcpy(entries[entries_index], trim(token));
                entries = realloc(entries, (++entries_index) * MAX_LABEL_SIZE * sizeof(char));
            } else {
                error_handler(SPACE_IN_WORD, line_number);
            }
        } else if (strstr(line, EXTERN_IDENTIFIER) != NULL) {
            /* TODO: BRUH*/
            token = strtok(line, s);
            token = strtok(NULL, s);
            if (check_for_spaces(trim(token))) {
                strcpy(externs[externs_index], trim(token));
                externs = realloc(externs, (++externs_index) * MAX_LABEL_SIZE * sizeof(char));
            } else {
                error_handler(SPACE_IN_WORD, line_number);
            }
            line_number++;
        }
    }
    fclose(fp);
}

//
//void object_output_extern (char *file_name, symbol *symbol_table, int symbol_table_size, char *symbol_name, int line_index) {
//    int i;
//    int j;
//
//    char str[80];
//
//    FILE *fp = fopen(file_name, "w");
//
///*strcpy(ext_file, file_name);
//strcat(ext_file, .ent);*/
//    for (i = 0; i < symbol_table_size; i++){
//        for (j = 0; j < symbol_table->attributes_size; j++){
//            if (symbol_table->attributes[i] == EXTERNAL_ATTRIBUTE){
//                sprintf(str, "%s %d %d\n",symbol_table->symbol_name, line_index, line_index + 1);
//                fputs(str, fp);
//            }
//        }
//    }
//}


void check_label_or_label_register(char *parm, int **table, symbol *symbol_table, int symbol_table_size, int line_index,
                                   int *table_index_prefixes[MAX_TABLE_SIZE]) {
    int symbol_index;
    int index;
    symbol_index = check_in_symbol_table(symbol_table, symbol_table_size, parm);
    if ((check_valid_label_name(parm) && symbol_index != -1) || check_label_with_register(parm) != -1) {
        /* maybe line_index-1??? idk bruh lols*/
        /* TODO: check if thats okay like u do return and change rows, susssssssss*/

        /* TODO HERE FUNCTION FOR WRITING THE EXTERNAL FILE*/

        index = ((*table_index_prefixes[line_index]) - 100) + 2;
        *table[index] += symbol_table[symbol_index].base_address;
        *table[index] = insert_value_by_index(*table[index], R_INDEX, 1);
        *table[index + 1] += symbol_table[symbol_index].offset;
        *table[index + 1] = insert_value_by_index(*table[index + 1], R_INDEX, 1);
    }
}


