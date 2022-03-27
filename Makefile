.DEFAULT_GOAL := all

CC=gcc
MAIN_FLAGS=-Wall -ansi -pedantic
CFLAGS=-c $(MAIN_FLAGS)

string_operations.o: string_operations.c
	$(CC) $(CFLAGS) string_operations.c -o string_operations.o

utils.o: utils.c
	$(CC) $(CFLAGS) utils.c -o utils.o

preprocessor.o: preprocessor.c string_operations.o
	$(CC) $(CFLAGS) preprocessor.c string_operations.o -o preprocessor.o

assembler.o: assembler.c string_operations.o utils.o
	$(CC) $(CFLAGS) assembler.c string_operations.o utils.o -o assembler.o

output.o: output.c assembler.o
	$(CC) $(CFLAGS) output.c assembler.o -o output.o

assembler_passes.o: assembler_passes.c string_operations.o assembler.o utils.o
	$(CC) $(CFLAGS) assembler_passes.c assembler.o string_operations.o utils.o -o assembler_passes.o

error.o: error.c
	$(CC) $(CFLAGS) error.c -o error.o

main: main.c preprocessor.o assembler.o error.o output.o assembler_passes.o
	$(CC) -o main $(MAIN_FLAGS) preprocessor.o assembler.o output.o assembler_passes.o string_operations.o error.o main.c

clean:
	rm *.o main
all: main
