#ifndef ERROR_H
#define ERROR_H
#include "settings.h"
#include "stdio.h"

/* TODO (Eilon & Liraz): change ERRORS_COUNT whenever adding an error... */
#define ERRORS_COUNT 11
enum POSSIBLE_ERRORS {NO_ERROR, LINE_SIZE_LONGER_THAN_MAX, ENDM_NOT_FOUND, NOT_WHOLE_NUMBER, FILE_NOT_FOUND, UNDEFINED_OPERAND, SYMBOL_ALREADY_IN_SYMBOL_TABLE, UNDEFINED_INSTRUCTION_ERROR, INVALID_LABEL, FILE_NAME_WITH_DOT, SPACE_IN_WORD};

void error_handler(int return_code, int line_number);
#endif
