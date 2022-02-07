#include "stdio.h"
//#include "preprocessor.h"
#include "string_operations.h"

int main() {
    /*
    printf("Started preprocessor!\n");
    char *file_name = "test_file.txt";
    char *processed_file_name = "processed_test_file.txt";
    start_preprocessor(file_name, processed_file_name);
    printf("Ended preprocessor!\n");
     */
    char s[MAX_LINE_SIZE] = "123";
    printf("%d\n", is_whole_number(s));
}
