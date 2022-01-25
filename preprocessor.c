#include "preprocessor.h"

/**
 * @param macro_table
 * @param macro_table_size
 * @param field
 * @return index of macro in search table, -1 if not in table
 */
int check_in_macro_table(struct macro macro_table[MAX_FILE_SIZE], int macro_table_size, char field[]) {
#ifdef DEBUG
    printf("in check_in_macro_table, table size: %d, field: %s\n", macro_table_size, field);
#endif
    int i;
    for (i = 0; i < macro_table_size; i++) {
        if (strcmp(field, macro_table[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

char *read_macro(FILE *fp) {
#ifdef DEBUG
    printf("In read_macro\n");
#endif
    char t_line[MAX_LINE_SIZE];
    char line[MAX_LINE_SIZE];
    char *tmp_line_to_trim;
    char *macro_code = (char *) malloc((MAX_LINE_SIZE + 1) * sizeof(char));
    while (fgets(t_line, MAX_LINE_SIZE, fp)) {
        clear_white_space(t_line, line);
        tmp_line_to_trim = (char *) malloc(strlen(line) * sizeof(char) + 1);
        strcpy(tmp_line_to_trim, line);
        if (strcmp(rtrim(tmp_line_to_trim), "endm") == 0) {
            return macro_code;
        }
        strcat(macro_code, line);
        macro_code = realloc(macro_code, (MAX_LINE_SIZE + 1) * sizeof(char));
    }
    return macro_code;
}

int start_preprocessor(char *file_name, char *processed_file_name) {
#ifdef DEBUG
    printf("Started preprocessor\n");
#endif
    char line[MAX_LINE_SIZE];
    /* for finding macros */
    char first_field[MAX_LINE_SIZE];
    char second_field[MAX_LINE_SIZE];
    /* macro table */
    struct macro macro_table[MAX_FILE_SIZE];
    int macro_table_index = 0;

    int index_in_macro_table;
    char *tmp_macro_code;
    FILE *fp = fopen(file_name, "r");
    FILE *new_fp = fopen(processed_file_name, "w");
    /* reading line by line */
    while (fgets(line, MAX_LINE_SIZE, fp)) {
        /* gets first field in line to check if in macro table or is "macro" */
        get_first_field(line, first_field);
        get_second_field(line, second_field);
        if (strcmp(first_field, "macro") == 0) {
            strcpy(macro_table[macro_table_index].name, second_field);
            tmp_macro_code = read_macro(fp);
            macro_table[macro_table_index].code = malloc(strlen(tmp_macro_code) * sizeof(char) + 1);
            strcpy(macro_table[macro_table_index].code, tmp_macro_code);
            macro_table_index++;
        } else {
            index_in_macro_table = check_in_macro_table(macro_table, macro_table_index, first_field);
            if (index_in_macro_table != -1) {
                fputs(macro_table[index_in_macro_table].code, new_fp);
            } else {
                fputs(line, new_fp);
            }
        }
    }
    /* maybe add enum for 'EXIT_CODE' ? */
    return 0;
}
// TODO (Eilon): add free function for macro table and other stuff

