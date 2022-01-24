#include "preprocessor.h"

void get_first_field(char line[], char field[]) {
    int line_i = 0;
    int field_i = 0;

    while (isspace(line[line_i++]));
    line_i--;
    while(!isspace(line[line_i])) {
        field[field_i++] = line[line_i++];
    }
    field[field_i] = '\0';
}

void get_second_field(char line[], char field[]) {
    int line_i = 0;
    int field_i = 0;

    while (isspace(line[line_i++]));
    line_i--;
    while(!isspace(line[line_i++]));
    line_i--;
    while (isspace(line[line_i++]));
    line_i--;
    while(!isspace(line[line_i])) {
        field[field_i++] = line[line_i++];
    }
    field[field_i] = '\0';
}

int check_in_macro_table(char macro_table[MAX_FILE_SIZE][MAX_LINE_SIZE], int macro_table_size, char field[]) {
    int i;
    for (i = 0; i < macro_table_size; i++) {
        if (strcmp(field, macro_table[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void copy_macro_to_file(char macro[MAX_FILE_SIZE][MAX_LINE_SIZE], FILE *fp) {
    // NOT FINAL, need to agree on final implementation for saving the macros
    int i;
    for (i = 0; i < MAX_FILE_SIZE; i++) {
        fputs(macro[i], fp);
    }
}

int start_preprocessor() {
    // TODO (Eilon): continue the algorithm in page 30
    // TODO (Eilon): also decide on how to save the macros text so it could copy efficiently

    char line[MAX_LINE_SIZE];

    char first_field[MAX_LINE_SIZE]; // for finding macros
    char second_field[MAX_LINE_SIZE]; // for finding macros

    char macro_table[MAX_FILE_SIZE][MAX_LINE_SIZE]; // macro name table
    int macro_table_index = 0;

    FILE *fp = fopen("test_file.txt","rw"); // TODO (Eilon): check if there's 'a' for appending

    while(fgets(line, MAX_LINE_SIZE, fp)) { // reading line by line
        get_first_field(line, first_field); // gets first field in line to check if in macro table or is "macro"
//        printf("%s,\t %s\n, %d", line, first_field, strcmp(first_field, "macro"));
        if (strcmp(first_field, "macro") == 0) {
            printf("found new macro!\n");
            get_second_field(line, second_field);
            strcpy(macro_table[macro_table_index++], second_field);

            // TODO (Eilon): add function read until "endm"

        }
        else if (check_in_macro_table(macro_table, macro_table_index, first_field)) {
            printf("place macro here!\n");
            // TODO (Eilon): add function copy_macro_to_file() in the current line
        }
    }

    return 0; // maybe add enum for 'EXIT_CODE' ?
}

int main() {
    start_preprocessor();
}

