#include "error.h"

void print_error(error err) {
    printf("ERROR (line %d): %s", err.line, err.error_type);
}
