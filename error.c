#include "error.h"

char errors_list[ERRORS_COUNT][MAX_LINE_SIZE] = {"Line is longer than MAX_LINE_SIZE", "'endm' was not found!", "A not whole number was found!", "File was not found!", "Undefined operand!", "Symbol is already in the symbol table!", "Undefined instruction!", "Invalid label!"};
void error_handler(int return_code, int line_number) {
    if (return_code == NO_ERROR) {
        return;
    }
    printf("ERROR (line %d): %s", line_number, errors_list[return_code - 1]);
}
