#include "assembler.h"

enum line_types {EMPTY_LINE, COMMENT, INSTRUCTION_LINE, COMMAND_LINE};
enum instruction_types {DATA_INSTRUCTION, STRING_INSTRUCTION, ENTRY_INSTRUCTION, EXTERN_INSTRUCTION};
char* instructions[] = {".data", ".string", ".entry", ".extern"};

typedef struct symbol {

} symbol;

int identify_line(char *line) {
    char *tmp;
    tmp = (char*) malloc(strlen(line)*(char) + 1)
    strcpy(tmp, line);
    if (strlen(trim(tmp)) == 0) {
        return EMPTY_LINE;
    }
    if (line[0] == ';') {
        return COMMENT;
    }
    int i;
    /* 4 is the length of instructions array TODO: maybe add length?? */
    /*
     * TODO: need to give out error if not a valid instruction!!!!!
     * */
    for (i = 0; i < 4; i++) {
        if (strstr(line, instructions[i]) != NULL) {
            /*
             * TODO (Eilon): is it possible to use:
             * return i;
             * although we have the enum, it's easier to use return i;
             * */
            switch (i) {
                case 0:
                    return DATA_INSTRUCTION;
                    break;
                case 1:
                    return STRING_INSTRUCTION;
                    break;
                case 2:
                    return ENTRY_INSTRUCTION;
                    break;
                case 3:
                    return EXTERN_INSTRUCTION;
                    break;
            }
        }
    }
    /* IDK if that's correct... */
    return COMMAND_LINE;
}

