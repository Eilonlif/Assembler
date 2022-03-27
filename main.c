#include "main.h"

void handle_input(char* file_name, int *input_flag) {
    if (check_no_dot(file_name)) {
        strcat(file_name, AS_FILE_ENDING);
    } else {
        error_handler(FILE_NAME_WITH_DOT, 0);
        (*input_flag) = FALSE;
    }
}

int assemble(char *file_name) {
    char processed_file_name[MAX_LINE_SIZE];
    /* Removing the '.as'*/
    file_name[strlen(file_name) - 3] = '\0';
    strcpy(processed_file_name, file_name);
    strcat(processed_file_name, AM_FILE_ENDING);
    strcat(file_name, AS_FILE_ENDING);
    start_preprocessor(file_name, processed_file_name);

    assembler(processed_file_name);
    return 0;
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
            assemble(argv[i]);
        }
    }

    return 0;
}
