#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *ltrim(char *s);

char *rtrim(char *s);

char *trim(char *s);

void get_first_field(char line[], char field[]);

void get_second_field(char line[], char field[]);

void clear_white_space(char line[], char tmp_line[]);