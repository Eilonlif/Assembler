#include "assembbler_passes.h"


void assembler_pass_1(char *file_name, int *table, int *table_index_prefixes[MAX_TABLE_SIZE], symbol *symbol_table) {


    *table_index_prefixes[0] = 100;
    int prefixes_index = 0;
    int table_index;
    table_index = 0;

    int L;

    char **externs = malloc(MAX_LABEL_SIZE * sizeof(char));
    char **entries = malloc(MAX_LABEL_SIZE * sizeof(char));
//    get_externs_and_entries(file_name, externs, entries);


    char line[MAX_LINE_SIZE];
    FILE *fp = fopen(file_name, "r");
    int symbol_def_flag;
    int line_identification;
    int symbol_table_size = 0;

    char *operand_names_table[] = {"mov", "cmp", "add", "sub", "lea", "clr", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    int n_fields = 2;
    char* fields[n_fields];

    int line_number = 1;

    int attributes[4];
    clear_values(attributes, 4);



    symbol_table_size = 1;


    symbol_table = malloc(sizeof(symbol));
    /* TODO: SUS */
    clear_symbol_for_symbol_table(symbol_table[0]);

    symbol_def_flag = FALSE;


    IC = 100;
    DC = 0;
    while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
        symbol_def_flag = strlen(has_label(line)) > 0;

        line_identification = identify_line(line);  /* line identification */
        if (line_identification == UNDEFINED_INSTRUCTION) {
            error_handler(UNDEFINED_INSTRUCTION, line_number);
        } else {
            get_first_n_fields(line, n_fields, fields); /* getting first n fields */
            if (line_identification == DATA_INSTRUCTION || line_identification == STRING_INSTRUCTION) {
                if (symbol_def_flag) {
                    attributes[0] = DATA_ATTRIBUTE;
                    insert_to_symbol_table(symbol_table, &symbol_table_size,
                                           fields[1], IC , IC - (IC % 16), (IC % 16),
                                           attributes, 1);
                    /* TODO (Eilon): Still need to do step 7... */
                }
            } else if (line_identification == ENTRY_INSTRUCTION) {/* step 8 */
                /* TODO (Eilon): indexing could be wrong... because idk what's with the label*/
                /* TODO (Eilon): divide into 2 lines...*/
                attributes[0] = EXTERNAL_ATTRIBUTE;
                insert_to_symbol_table(symbol_table, &symbol_table_size,
                                       fields[1], 0, 0, 0,
                                       attributes, 1);
                /* here go back to step 2... */
            } else {
                if (symbol_def_flag) {
                    if (!check_in_operand_table(operand_names_table, fields[1])) {
                        error_handler(UNDEFINED_OPERAND, line_number);
                    } else if (!check_valid_label(fields[0])) {
                        /* TODO (Eilon): do you need to add check if in symbol table? */
                        error_handler(INVALID_LABEL, line_number);
                    } else {
                        /* TODO (Eilon): else?? */
                        attributes[0] = CODE_ATTRIBUTE;
                        insert_to_symbol_table(symbol_table, &symbol_table_size,
                                               fields[2], IC, IC - (IC % 16), (IC % 16),
                                               attributes, 1);
                    }
                }
                L = calculate_binary_code(line,operand_names_table, table, &table_index);
                IC += L;
                *table_index_prefixes[prefixes_index++] = IC;
            }

        }
        line_number++;
    }
}


void assembler_pass_2(char *file_name, int** table, symbol** symbol_table, int symbol_table_size, int *table_index_prefixes[MAX_TABLE_SIZE]) {
    char line[MAX_LINE_SIZE];
    FILE *fp = fopen(file_name, "r");
    int line_id;

    int n_fields = 2;
    char* fields[n_fields];

    int symbol_index;

    char *parm1;
    char *parm2;

    parm1 = (char*) malloc(MAX_LINE_SIZE * sizeof(char));
    parm2 = (char*) malloc(MAX_LINE_SIZE * sizeof(char));

    char *values;

    char label[MAX_LABEL_SIZE];



    int line_index;
    line_index = 0;

    while (fgets(line, MAX_LINE_SIZE, fp) != NULL) {
        line_id = identify_line(line);
        if (line_id != COMMENT && line_id != EMPTY_LINE) {
            line_index++;
        }
        if(!(line_id == DATA_INSTRUCTION || line_id == STRING_INSTRUCTION || line_id == EXTERN_INSTRUCTION)) {
            if(line_id == ENTRY_INSTRUCTION) {
                /* step 5 */
                get_first_n_fields(line, n_fields, fields);
                symbol_index = check_in_symbol_table(*symbol_table, symbol_table_size, fields[1]);
                if(symbol_index != -1) {
                    symbol_table[symbol_index]->attributes[(symbol_table[symbol_index]->attributes_size)++] = ENTRY_ATTRIBUTE;
                } else {
                    /*TODO:erorr*/
                }
            } else {
                /* step 6 */
                strcpy(label, has_label(line));
                split_by_comma(line, &values, strlen(label) == 0);
                strcpy(parm1, &values[2 * MAX_LINE_SIZE]);
                strcpy(parm2, &values[3 * MAX_LINE_SIZE]);
                check_label_or_label_register(parm1, table, symbol_table, symbol_table_size, line_index, table_index_prefixes);
                check_label_or_label_register(parm2, table, symbol_table, symbol_table_size, line_index, table_index_prefixes);

            }
        }
    }
}

void assembler(char *file_name) {
    int table[MAX_TABLE_SIZE];
    int table_index_prefixes[MAX_TABLE_SIZE];
    symbol *symbol_table;


}