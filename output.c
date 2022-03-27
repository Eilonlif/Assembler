#include "output.h"

void object_output(char *file_name, int* table, int table_size) {
    int i;
    int nibble_idx;
    int NIBBLE_SIZE = 4;
    char hex_chars[16][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
    char starting_chars[5][2] = {"A", "B", "C", "D", "E"};
    char line[26] = {'\0'};

    char object_file_name[MAX_LINE_SIZE];
    strcpy(object_file_name, file_name);

    object_file_name[strlen(object_file_name) - 3] = '\0';
    strcat(object_file_name, ".ob");
    FILE *fp = fopen(object_file_name, "w");
    char final_line[MAX_LINE_SIZE];
    char zero[2] = {'0', '\0'};

    char str[MAX_LINE_SIZE];
    sprintf(str,"%d %d\n", ICF-100-DCF -1, DCF + 1);
    fputs(str, fp);

    for (i = 0; i < table_size; i++) {
        if (table[i] > 0) {
            strcpy(line, "");
            int tmp_integer = table[i];
            unsigned char tmp_nibble;
            for (nibble_idx = 5 - 1; nibble_idx >= 0; nibble_idx--) {
                strcat(line, starting_chars[(5 - 1) - nibble_idx]);
                tmp_nibble = (tmp_integer >> (NIBBLE_SIZE * nibble_idx)) & 0x0f;
                strcat(line, hex_chars[tmp_nibble]);
                if (nibble_idx > 0) {
                    strcat(line, "-");
                }
            }

            strcat(line, "\n");

            sprintf(final_line, "%c%d %s", i + 100 > 1000 ? 0: '0', i + 100, line);
            fputs(final_line, fp);
        }
    }
}

void entries_output (char *file_name, symbol *symbol_table, int symbol_table_size) {
    int i;
    int j;
    char str[MAX_LINE_SIZE*symbol_table_size];
    char entry_file_name[MAX_LINE_SIZE];
    strcpy(entry_file_name, file_name);

    entry_file_name[strlen(entry_file_name) - 3] = '\0';
    strcat(entry_file_name, ".ent");

    FILE *fp = fopen(entry_file_name, "w");

    for (i = 0; i < symbol_table_size; i++) {
        for (j = 0; j < symbol_table[i].attributes_size + 1; j++) {
            if (symbol_table[i].attributes[i] == ENTRY_ATTRIBUTE){
                sprintf(str,"%s,%d,%d\n",(symbol_table[i]).symbol_name, symbol_table[i].base_address, symbol_table[i].offset);
                fputs(str, fp);
            }
        }
    }
    fclose(fp);
}

