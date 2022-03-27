#include "utils.h"

void debug(char *debug_msg) {
#ifdef DEBUG
    printf("%s", debug_msg);
#endif
}

#include <stdlib.h>
#include <limits.h>


/* for printing a binary number */
//char * int2bin(int i) {
void int2bin(int i) {
    size_t bits = sizeof(int) * CHAR_BIT;

    char * str = calloc(bits + 1, sizeof(char));
//    if(!str) return NULL;
    if(!str) return;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';
    printf("bin row: %s\n", str);
//    return str;
}
