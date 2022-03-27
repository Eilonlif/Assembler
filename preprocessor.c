#include "preprocessor.h"

/* TODO (Eilon & Liraz): Also separate the preprocessor algorithm implementation to a different file?? */

/**
 * @param macro_table
 * @param macro_table_size
 * @param field
 * @return index of macro in search table, MACRO_NOT_FOUND if not in table
 */
 /* TODO (Eilon): change to 'macro *macro_table' after you changed it to be malloced... */
int check_in_macro_table(macro macro_table[MAX_FILE_SIZE], int macro_table_size, char field[]) {
    int i;
    for (i = 0; i < macro_table_size; i++) {
        if (strcmp(field, macro_table[i].name) == 0) {
            return i;
        }
    }
    return MACRO_NOT_FOUND;
}

char *read_macro(FILE *fp) {
    char t_line[MAX_LINE_SIZE];
    char line[MAX_LINE_SIZE];
    char *macro_code = (char *) malloc((MAX_LINE_SIZE + 1) * sizeof(char));
    int line_counter = 1;
    if (macro_code == NULL) {
        return NULL;
    }
    while (fgets(line, MAX_LINE_SIZE, fp)) {
        strcpy(t_line, trim(line));
        if (strcmp(t_line, MACRO_END) == 0) {
            return macro_code;
        }
        /* ignoring empty lines */
        if (strlen(t_line) != 0) {
            strcat(t_line, "\n");
        }
        strcat(macro_code, t_line);
        macro_code = realloc(macro_code, ((MAX_LINE_SIZE + 1) * (line_counter++)) * sizeof(char));
    }
    return macro_code;
}

int start_preprocessor(char *file_name, char *processed_file_name) {
    char line[MAX_LINE_SIZE];
    /* for finding macros */
    int n_fields = 2;
    char* fields[n_fields];
    /* macro table */
    macro macro_table[MAX_FILE_SIZE];
    int macro_table_index = 0;
    int index_in_macro_table;
    FILE *fp = fopen(file_name, "r");
    FILE *new_fp = fopen(processed_file_name, FILE_WRITE_MODE);
    /* reading line by line */
    while (fgets(line, MAX_LINE_SIZE, fp)) {
        /* gets first field in line to check if in macro table or is "macro" */
        get_first_n_fields(line, n_fields, fields);
        /* checking if line is not empty */
        if (fields[0] != NULL) {
            if (strcmp(fields[0], MACRO_START) == 0) {
                strcpy(macro_table[macro_table_index].name, fields[1]);
                macro_table[macro_table_index].code = read_macro(fp);
                macro_table_index++;
            } else {
                index_in_macro_table = check_in_macro_table(macro_table, macro_table_index, fields[0]);
                if (index_in_macro_table != MACRO_NOT_FOUND) {
                    fputs(macro_table[index_in_macro_table].code, new_fp);
                } else {
                    fputs(line, new_fp);
                }
            }
        }
    }
    fclose(fp);
    fclose(new_fp);
    /* maybe add enum for EXIT_CODE ? */
    return NO_ERROR;
}
