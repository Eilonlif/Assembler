#include "utils.h"

void debug(char *debug_msg) {
#ifdef DEBUG
    printf("%s", debug_msg);
#endif
}
