#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void object_output(char *file_name, int* table, int table_size) {
    int i;
    int nibble_idx;
    int NIBBLE_SIZE = 4;
    char hex_chars[16][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
    char starting_chars[5][2] = {"A", "B", "C", "D", "E"};
    char line[26] = {'\0'};
    int l[] = {1493, 9348, 1344, 9999, 4096-1};
    for (i = 0; i < 5; i++) {
        strcpy(line, "");
        int tmp_integer = l[i];
        unsigned char tmp_nibble;
        for (nibble_idx = 5 - 1; nibble_idx >= 0; nibble_idx--) {
            strcat(line, starting_chars[(5 - 1) - nibble_idx]);
            tmp_nibble = (tmp_integer >> (NIBBLE_SIZE * nibble_idx)) & 0x0f;
            strcat(line, hex_chars[tmp_nibble]);
            if (nibble_idx > 0) {
                strcat(line, "-");
            }
        }
        /*fputs*/
        printf("%s\n", line);
    }
}
//
//void object_output_entries (char *file_name, char **entries, symbol *symbol_table, int symbol_table_size, char *symbol_name) {
//
//    int i;
//    int j;
//
//    char str[80];
//
//    FILE *fp = fopen(file_name, "w");
//
//    for (i = 0; i < symbol_table_size; i++){
//
//        for (j = 0; j < symbol_table[i].attributes_size; j++){
//
//            if (symbol_table[i].attributes[i] == ENTRY_ATTRIBUTE){
//                sprint(str,"%s,%d,%d\n",symbol_table->symbol_name,symbol.base_adress,symbol.offset);
//                fputs(str, fp);
//            }
//
//        }
//
//    }
//}


int main() {

}
