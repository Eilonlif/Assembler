cmake_minimum_required (VERSION 2.8.11)
set(CMAKE_CXX_STANDARD 14)
project (mmn14)

set(CC gcc)
set(MAIN_FLAGS -Wall -ansi -pedantic)
set(CFLAGS -c ${MAIN_FLAGS})

#add_executable(string_operations.o string_operations.c)
#add_executable(utils.o  utils.c)
#add_executable(preprocessor.o preprocessor.c)
#add_executable(assembler.o assembler.c)
#add_executable(error.o error.c)


add_executable (main main.c string_operations.c utils.c preprocessor.c assembler_passes.c assembler.c error.c output.c)

#
#string_operations.o: string_operations.c
#$(CC) $(CFLAGS) string_operations.c -o string_operations.o
#preprocessor.o: preprocessor.c string_operations.o
#$(CC) $(CFLAGS) preprocessor.c -o preprocessor.o
#assembler.o: assembler.c string_operations.o
#$(CC) $(CFLAGS) assembler.c -o assembler.o
#error.o: error.c
#$(CC) $(CFLAGS) error.c -o error.o
#main: main.c preprocessor.o assembler.o error.o
#$(CC) -o main $(MAIN_FLAGS) assembler.o string_operations.o preprocessor.o error.o main.c

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
