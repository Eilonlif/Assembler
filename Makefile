.DEFAULT_GOAL := main
string_operations.o: string_operations.c
	gcc -c -Wall -ansi -pedantic string_operations.c -o string_operations.o
preprocessor.o: preprocessor.c
	gcc -c -Wall -ansi -pedantic preprocessor.c -o preprocessor.o
main.o: main.c
	gcc -c -Wall -ansi -pedantic main.c -o main.o
main: main.o preprocessor.o
	gcc -Wall -pedantic main.o preprocessor.o string_operations.c -o main
clean:
	rm magic main.o preprocessor.o string_operations.o
