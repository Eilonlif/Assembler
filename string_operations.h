#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "settings.h"
#include "error.h"
#include "utils.h"


#define QUOTES_IDENTIFIER '"'
#define COMMA_IDENTIFIER ','
#define DOT_IDENTIFIER '.'


char *ltrim(char *s);

char *rtrim(char *s);

char *trim(char *s);

void get_first_n_fields(char line[], int n, char** field_array);

void clear_white_space(char line[], char tmp_line[]);

int is_whole_number(char tmp_line[]);

char* delete_first_n_chars(char* line, int n);

short check_no_dot(char line[]);

#endif
