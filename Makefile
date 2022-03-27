.DEFAULT_GOAL := all

CC=gcc
MAIN_FLAGS=-Wall -ansi -pedantic
CFLAGS=-c $(MAIN_FLAGS)

string_operations.o: string_operations.c
	$(CC) $(CFLAGS) string_operations.c -o string_operations.o

preprocessor.o: preprocessor.c string_operations.o
	$(CC) $(CFLAGS) preprocessor.c -o preprocessor.o

assembler.o: assembler.c string_operations.o
	$(CC) $(CFLAGS) assembler.c -o assembler.o

output.o: output.c assembler.o
	$(CC) $(CFLAGS) output.c -o output.o

assembler_passes.o: assembler_passes.c assembler.o
	$(CC) $(CFLAGS) assembler_passes.c -o assembler_passes.o

error.o: error.c
	$(CC) $(CFLAGS) error.c -o error.o

main: main.c preprocessor.o assembler.o error.o output.o assembler_passes.o
	$(CC) -o main $(MAIN_FLAGS) assembler.o output.o assembler_passes.o string_operations.o preprocessor.o error.o main.c

clean:
	rm *.o main
all: main
