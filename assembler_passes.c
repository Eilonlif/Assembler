#include "assembler_passes.h"
char *operand_names_table[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
                               "jsr", "red", "prn", "rts", "stop"};
void assembler_pass_1(char *file_name, int **table, int *table_index_prefixes[MAX_TABLE_SIZE], symbol **symbol_table,
                      int *symbol_table_size) {


    (*table_index_prefixes)[0] = 100;
    int prefixes_index = 1;
    int table_index;
    table_index = 0;

    int L;

    char *externs = calloc(MAX_LABEL_SIZE, sizeof(char));
    char *entries = calloc(MAX_LABEL_SIZE, sizeof(char));
    char *labels = calloc(MAX_LABEL_SIZE, sizeof(char));
    int externs_size;
    int labels_size;

    get_externs_and_entries_and_labels(file_name, &externs, &entries, &externs_size, &labels, &labels_size);

    char line[MAX_LINE_SIZE * 2];
    FILE *fp = fopen(file_name, "r");
    int symbol_def_flag;
    int line_identification;

    int n_fields = 3;
    char *fields[n_fields];

    int line_number = 1;
    int attributes[4];
    clear_values(attributes, 4);

    *symbol_table_size = 1;

    *symbol_table = (symbol*) malloc(sizeof(symbol));

    symbol_def_flag = FALSE;

    IC = 100;
    DC = 0;
    while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
        line_identification = identify_line(line);  /* line identification */
        /* TODO WHAT ABOUT search in symnol table with :*/

        if (line_identification == UNDEFINED_INSTRUCTION) {
            error_handler(UNDEFINED_INSTRUCTION_ERROR, line_number);
        } else if (!(line_identification == COMMENT || line_identification == EMPTY_LINE)) {
            symbol_def_flag = strlen(has_label(line)) > 0;
            get_first_n_fields(line, n_fields - 1, fields); /* getting first n fields */
            if (line_identification == DATA_INSTRUCTION || line_identification == STRING_INSTRUCTION) {
                if (symbol_def_flag) {
                    attributes[0] = DATA_ATTRIBUTE;
                    fields[0][strlen(fields[0]) - 1] = '\0';
                    insert_to_symbol_table(symbol_table, symbol_table_size,
                                           fields[0], IC, IC - (IC % 16), (IC % 16),
                                           attributes, 1);
                }
                L = calculate_binary_code(line, operand_names_table, *table, &table_index, *symbol_table, *symbol_table_size, externs, externs_size, labels, labels_size);
                IC += L;
                (*table_index_prefixes)[prefixes_index++] = IC;
            } else if (line_identification == EXTERN_INSTRUCTION) { /* step 8 */
                attributes[0] = EXTERNAL_ATTRIBUTE;
                if (fields[1][0] == INSTRUCTION_IDENTIFIER) {
                    insert_to_symbol_table(symbol_table, symbol_table_size,
                                           fields[2], 0, 0, 0,
                                           attributes, 1);

                } else {
                    insert_to_symbol_table(symbol_table, symbol_table_size,
                                           fields[1], 0, 0, 0,
                                           attributes, 1);
                }
            } else { /* step 11*/
                if (symbol_def_flag) {
                    if (!check_in_operand_table(operand_names_table, fields[1])) { /* step 12*/
                        error_handler(UNDEFINED_OPERAND, line_number);
                    } else if (!check_valid_label(fields[0])) {
                        error_handler(INVALID_LABEL, line_number);
                    } else {
                        attributes[0] = CODE_ATTRIBUTE;
                        fields[0][strlen(fields[0]) - 1] = '\0';
                        insert_to_symbol_table(symbol_table, symbol_table_size,
                                               fields[0], IC, IC - (IC % 16), (IC % 16),
                                               attributes, 1);
                    }
                }
                /* step 13 - 16*/
                if (line_identification != ENTRY_INSTRUCTION) {
                    L = calculate_binary_code(line, operand_names_table, *table, &table_index, *symbol_table, *symbol_table_size, externs, externs_size, labels, labels_size);
                    IC += L;
                    (*table_index_prefixes)[prefixes_index++] = IC;
                }
            }
        }
        line_number++;
    }
    DCF = DC;
    ICF = IC;
}



void assembler_pass_2(char *file_name, int **table, int *table_index_prefixes[MAX_TABLE_SIZE], symbol **symbol_table,
                      int *symbol_table_size) {
    char line[MAX_LINE_SIZE];
    FILE *fp = fopen(file_name, "r");
    int line_id;

    int n_fields = 2;
    char *fields[n_fields];

    int symbol_index;

    char *parm1;
    char *parm2;

    char *values;
    int values_size;

    char label[MAX_LABEL_SIZE];


    int line_index;
    line_index = 0;
    int k;


    parm1 = (char *) calloc(MAX_LINE_SIZE, sizeof(char));
    parm2 = (char *) calloc(MAX_LINE_SIZE, sizeof(char));

    for (k = 0; k < 50; k++) {
        printf("%d:\t%d\n", k, (*table_index_prefixes)[k]);
    }

    while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
        line_id = identify_line(line);
        if (line_id != COMMENT && line_id != EMPTY_LINE && line_id != EXTERN_INSTRUCTION && line_id != ENTRY_INSTRUCTION) {
            line_index++;
        }
        if (!(line_id == DATA_INSTRUCTION || line_id == STRING_INSTRUCTION || line_id == EXTERN_INSTRUCTION) &&
            (line_id != COMMENT && line_id != EMPTY_LINE)) {
            if (line_id == ENTRY_INSTRUCTION) {
                /* step 5 */
                get_first_n_fields(line, n_fields, fields);
                symbol_index = check_in_symbol_table(*symbol_table, *symbol_table_size, fields[1]);
                if (symbol_index != -1) {
                    /* TODO might be wrong*/
                    (*symbol_table)[symbol_index].attributes[((*symbol_table)[symbol_index].attributes_size)++] = ENTRY_ATTRIBUTE;
                } else {
                    /*TODO:erorr*/
                }
            } else {
                /* step 6 */
                strcpy(label, has_label(line));
                split_by_comma(line, &values, strlen(label) > 0, &values_size);
                strcpy(parm1, &(values[2 * MAX_LINE_SIZE]));
                strcpy(parm2, &(values[3 * MAX_LINE_SIZE]));

                int op_code = -1;
                int i;
                for (i = 0; i < OPERAND_NAMES_TABLE_SIZE; i++) {
                    if (strcmp(&(values[MAX_LINE_SIZE]), operand_names_table[i]) == 0) {
                        op_code = op_table[i][0];
                    }
                }

                if (op_code < 5) {
                    check_label_or_label_register(parm1, table, *symbol_table, *symbol_table_size, line_index - 1,
                                                  table_index_prefixes, file_name);
                }
                if (op_code < 14) {
                    check_label_or_label_register(op_code > 4 ?parm1 : parm2, table, *symbol_table, *symbol_table_size, line_index - 1,
                                                  table_index_prefixes, file_name);
                }
            }
        }
    }
}

void assembler(char *file_name) {
    int *table;
    int *table_index_prefixes;
    symbol *symbol_table;
    int symbol_table_size;

    table = (int *) malloc(MAX_TABLE_SIZE * sizeof(int));
    table_index_prefixes = (int *) malloc(MAX_TABLE_SIZE * sizeof(int));
    assembler_pass_1(file_name, &table, &table_index_prefixes, &symbol_table, &symbol_table_size);
    printf("starting pass 2\n");
    assembler_pass_2(file_name, &table, &table_index_prefixes, &symbol_table, &symbol_table_size);

    entries_output(file_name, symbol_table, symbol_table_size);
    object_output(file_name, table, MAX_TABLE_SIZE);

}
