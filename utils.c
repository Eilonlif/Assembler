#include "utils.h"

void debug(char *debug_msg) {
#ifdef DEBUG
    printf("%s", debug_msg);
#endif
}


/* for printing a binary number */
void int2bin(int i) {
    size_t bits = sizeof(int) * CHAR_BIT;
    char * str = calloc(bits + 1, sizeof(char));
    if(!str) return;
    str[bits] = 0;

    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';
    printf("bin row: %s\n", str);
}
