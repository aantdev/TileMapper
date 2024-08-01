#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEN_STD 256
#define SEARCH_RESET -1

#define isNum(arg)(arg>='0' && arg<='9')
#define isSym(arg)(strcmp(arg, ";"))

//take txt file, turn to char*
char* ftostr(char *path);

//check that token is valid
int check_valid(const char* op, const char** trgt, int limit);
//check that token is int
void check_int(const char* num);

#endif
