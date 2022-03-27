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
        field_array[i] = (char *) calloc(tmp_field_index, sizeof(char));
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
    while (isspace(line[line_index]) || line[line_index++] == '\n');
    line_index--;
    while (line[line_index] != '\0') {
        tmp_line[tmp_line_index++] = line[line_index++];
    }
    tmp_line[tmp_line_index] = '\0';
}

int is_whole_number(char tmp_line[]) {
    char line[MAX_LINE_SIZE];
    int line_i;
    strcpy(line, trim(tmp_line));

    line_i = 0;
    if (line[0] == '+' || line[0] == '-'){
        line_i++;
    }
    for (;line_i < strlen(line); line_i++) {
        if (isdigit(line[line_i]) != 1) {
            return FALSE;
        }
    }
    return TRUE;
}

char* delete_first_n_chars(char* line, int n) {
    /* not using delete_first_n_chars but fix it anyway... */
    /*        TODO: Memory leak! */
    /*    strcpy(line, delete_first_n_chars(fields[0], 1)); */
    int line_length = strlen(line);
    int tmp_index = 0;
    char* tmp = (char *) calloc(line_length, sizeof(char));
    int i;

    /* TODO (Eilon): check indexing... */
    /* Edge case */
    if (n > line_length) {
        return "";
    }
    for (i = n; i < line_length; i ++) {
        tmp[tmp_index++] = line[i];
    }
    return tmp;
}

short check_valid_quotes(char line[]) {
    int i;
    int quotes_flag = 0;
    for (i = 0; i < MAX_LINE_SIZE; i++) {
        if (line[i] == QUOTES_IDENTIFIER) {
            quotes_flag++;
        }
    }
    return quotes_flag == 0 || quotes_flag == 2;
}

short check_no_dot(char line[]) {
    int i;
    for (i = 0; i < MAX_LINE_SIZE; i++) {
        if (line[i] == DOT_IDENTIFIER) {
            return FALSE;
        }
    }
    return TRUE;
}


char* clean_spaces(char* line) {
    int i;
    char* tmp_line;
    tmp_line = (char*)calloc(MAX_LINE_SIZE * 2,sizeof(char));
    int tmp_line_index = 0;

    for (i = 0; i < strlen(line) && i < MAX_LINE_SIZE; i++) {
        if (line[i] == '\t') {
            line[i] = ' ';
        }
    }
    for (i = 0; i < strlen(line) - 1 && i < MAX_LINE_SIZE ; i++) {
        if (line[i] == ' ') {
            while (line[i + 1] == ' ') {
                i++;
            }
        }
        tmp_line[tmp_line_index++] = line[i];
    }
    return trim(tmp_line);
}






/* TODO (Eilon): add function for checking valid ',' */

/* label: op a,b, c, d */
/* TODO (Eilon & Liraz): do that @ friday / saturday */
/*
short check_valid_commas(char line[]) {
    int i;
    int comma_flag = FALSE;
    int last_space_index = 0;
    int values[10];  TODO (Eilon): IDK what's the size...
    if ()
    for (i = 0; i < MAX_LINE_SIZE; i++) {
        if (isspace(line[i])) {
            last_space_index = i;
        }
        if (line[i] == COMMA_IDENTIFIER) {
            comma_flag = TRUE;

        }
    }
}
*/
