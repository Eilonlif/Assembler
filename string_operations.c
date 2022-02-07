#include "string_operations.h"

/**
 * trims white spaces from the left of s
 * be careful: will change s!
 * @param s
 * @return
 */
char *ltrim(char *s) {
    while (isspace(*s)) s++;
    return s;
}

/**
 * trims white spaces from the right of s
 * be careful: will change s!
 * @param s
 * @return
 */
char *rtrim(char *s) {
    char *back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}

/**
 * trims white spaces from the right and from the left of s
 * be careful: will change s!
 * @param s
 * @return
 */
char *trim(char *s) {
    return rtrim(ltrim(s));
}

/**
 * get a list with the first n words in line
 * @param line
 * @param n
 * @param field_array
 */
void get_first_n_fields(char line[], int n, char** field_array) {
    int i;
    int line_i = 0;
    char tmp_field[MAX_LINE_SIZE];
    int tmp_field_index = 0;
    for (i = 0; i < n; i++) {
        field_array[i] = NULL;
    }
    for (i = 0; i < n; i++, tmp_field_index = 0) {
        while (isspace(line[line_i]) && line[line_i] != '\0' ) {
            if (line[line_i] == '\n') {
                return;
            }
            line_i++;
        }
        if (line_i >= strlen(line)) {
            return;
        }
        while (!isspace(line[line_i]) && line[line_i] != '\0' && line[line_i] != '\n') {
            tmp_field[tmp_field_index++] = line[line_i++];
        }
        tmp_field[tmp_field_index++] = '\0';
        field_array[i] = (char *) malloc(tmp_field_index * sizeof(char));
        strncpy(field_array[i], tmp_field, tmp_field_index);
    }
}

/**
 * like ltrim but not changing line
 * filling empty spaces with null
 * @param line
 * @param tmp_line
 */
void clear_white_space(char line[], char tmp_line[]) {
    int tmp_line_index = 0;
    int line_index = 0;
    while (isspace(line[line_index++]));
    line_index--;
    while (line[line_index] != '\0') {
        tmp_line[tmp_line_index++] = line[line_index++];
    }
    tmp_line[tmp_line_index] = '\0';
}