#include "error.h"

void print_error(char *error_msg) {
    printf("%s\n", error_msg);
}

void error_handler(int return_code, int line_number) {
    if (return_code == NO_ERROR) {
        return;
    }
    printf("(line %d): ", line_number);

    /* TODO (Liraz): add remaining errors... */
    switch (return_code) {
        case LINE_SIZE_LONGER_THAN_MAX:
            print_error("something....");
            break;
            /*TODO: add shit bruhthuer*/
        case SPACE_IN_WORD:
            print_error("Space in a field");
            break;
        case UNDEFINED_INSTRUCTION_ERROR:
            print_error("Undefined instruction");
            break;
        case UNDEFINED_OPERAND:
            print_error("Undefined operand");
            break;
        case INVALID_LABEL:
            print_error("Invalid label");
            break;
    }
}

