.DEFAULT_GOAL := all
string_operations.o: string_operations.c
	gcc -c -Wall -ansi -pedantic string_operations.c -o string_operations.o
preprocessor.o: preprocessor.c
	gcc -c -Wall -ansi -pedantic preprocessor.c -o preprocessor.o
assembler.o: assembler.c
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
error.o: error.c
	gcc -c -Wall -ansi -pedantic error.c -o errors.o
main.o: main.c
	gcc -c -Wall -ansi -pedantic main.c -o main.o
main: main.o preprocessor.o
	gcc -Wall -pedantic assembler.o main.o preprocessor.o string_operations.c -o main
clean:
	rm *.o main
all: error.o string_operations.o preprocessor.o assembler.o
