#include "main.h"

void handle_input(char* file_name, int *input_flag) {
    if (check_no_dot(file_name)) {
        /* TODO: Check this, sus... */
        strcat(file_name, AS_FILE_ENDING);
    } else {
        error_handler(FILE_NAME_WITH_DOT);
        (*input_flag) = FALSE;
    }
}

int assemble(char *file_name) {
    char processed_file_name[MAX_LINE_SIZE];
    int assembler_pass_1_result;
    int assembler_pass_2_result;
    /* Removing the '.as'*/
    file_name[strlen(file_name) - 4] = '\0'
    strcpy(processed_file_name, file_name);

    strcat(processed_file_name, AM_FILE_ENDING);

    start_preprocessor(file_name, processed_file_name);
    /* TODO: Currently returning void... and i dont remember if it shou,ld continue if an error was found in preprocessor */

    assembler_pass_1_result = assembler_pass_1(processed_file_name);
    if (assembler_pass_1_result) {
        /* assembler_pass_2_result = assembler_pass_2(); */
    }
    if (assembler_pass_2_result) {
        /*
        create_extern_file();
        create_entry_file()
        create_ob_file();
         */
    }
}

int main(int argc, char* argv[]) {
    int i;
    int input_flag = TRUE;
    if (argc <= 1){
        printf("No file passed \n");
        return 0;
    }

    for (i = 1; i < argc; i++){
        handle_input(argv[i], &input_flag);
        if (input_flag) {
            /* NOT sure its gonna work because im changing argv[i] in the other function... */
            assemble(argv[i]);
        }
    }

    return 0;
}

