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
 * get the first word in the line (ignoring white spaces)
 * @param line - the line
 * @param field - the field (empty at the start of the function)
 */
void get_first_field(char line[], char field[]) {
    int line_i = 0;
    int field_i = 0;

    while (isspace(line[line_i++]));
    line_i--;
    while (!isspace(line[line_i])) {
        field[field_i++] = line[line_i++];
    }
    field[field_i] = '\0';
}

/**
 * get the second word in the line (ignoring white spaces)
 * @param line - the line
 * @param field - the field (empty at the start of the function)
 */
void get_second_field(char line[], char field[]) {
    int line_i = 0;
    int field_i = 0;

    while (isspace(line[line_i++]));
    line_i--;
    while (!isspace(line[line_i++]));
    line_i--;
    while (isspace(line[line_i++]));
    line_i--;
    while (!isspace(line[line_i])) {
        field[field_i++] = line[line_i++];
    }
    field[field_i] = '\0';
}


/**
 * like ltrim but not changing line
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