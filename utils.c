#include "utils.h"


#include <stdlib.h>
#include <limits.h>


/**
 * helper function for debugging 
 * gets a string and print it to the screen 
 * @debug_msg string to print 
 */
void debug(char *debug_msg) {
#ifdef DEBUG
    printf("%s", debug_msg);
#endif
}



/**
 * helper function for debugging
 * convert a int number in Decimal basis (10) to a Binary basis (2)
 * @param i the int number to convert 
 * @return str string that contains i in binary 
 */
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
