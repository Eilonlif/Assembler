#ifndef ERROR_H
#define ERROR_H
#include "settings.h"
#include "stdio.h"

#define ERRORS_COUNT 11
enum POSSIBLE_ERRORS {NO_ERROR, LINE_SIZE_LONGER_THAN_MAX, ENDM_NOT_FOUND, NOT_WHOLE_NUMBER, FILE_NOT_FOUND, UNDEFINED_OPERAND, SYMBOL_ALREADY_IN_SYMBOL_TABLE, UNDEFINED_INSTRUCTION, INVALID_LABEL};

void error_handler(int return_code, int line_number);
#endif
