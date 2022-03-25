#include "utils.h"

void debug(char *debug_msg) {
#ifdef DEBUG
    printf("%s", debug_msg);
#endif
}

#include <stdlib.h>
#include <limits.h>


/* for printing a binary number */
char * int2bin(int i)
{
    size_t bits = sizeof(int) * CHAR_BIT;

    char * str = malloc(bits + 1);
    if(!str) return NULL;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    return str;
}
