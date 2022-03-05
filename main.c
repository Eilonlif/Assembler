//#include "assembler.h"
#include <stdio.h>
#include <string.h>
int main() {
    /*
    printf("Started preprocessor!\n");
    char *file_name = "test_file.txt";
    char *processed_file_name = "processed_test_file.txt";
    start_preprocessor(file_name, processed_file_name);
    printf("Ended preprocessor!\n");
     */

    char *fname = "fname";
    char ff[10 + 4];
    strcpy(ff, fname);
    strcat(ff, ".as");
//    strcpy(ff + strlen(fname), ".as");
    printf("%s\n", ff);
//    char line[100];
//    FILE *fp = fopen(ff, "r");
//    fgets(line, 100, fp);
//    printf("%s\n", line);
}


