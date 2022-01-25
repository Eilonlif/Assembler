#include "stdio.h"
#include "preprocessor.h"

int main() {
    printf("Started preprocessor!\n");
    char *file_name = "test_file.txt";
    char *processed_file_name = "processed_test_file.txt";
    start_preprocessor(file_name, processed_file_name);
    printf("Ended preprocessor!\n");
}
/*
 *
 * .DEFAULT_GOAL := magic
magic_square.o: magic_square.c
	gcc -c -Wall -ansi -pedantic magic_square.c -o magic_square.o
magic.o: magic.c
	gcc -c -Wall -ansi -pedantic magic.c -o magic.o
magic: magic.o magic_square.o
	gcc -Wall -pedantic magic.o magic_square.o -o magic
clean:
	rm magic magic.o magic_square.o

 *
 *
 *
 * */