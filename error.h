#include "stdio.h"

typedef struct error {
    char *error_type;
    int line;
} error;

#define LINE_SIZE_LONGER_THAN_MAX "Line is longer than MAX_LINE_SIZE"
#define ENDM_NOT_FOUND "'endm' was not found!"
#define NOT_WHOLE_NUMBER "A not whole number was found!"
#define FILE_NOT_FOUND "File was not found!"