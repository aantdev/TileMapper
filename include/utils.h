#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEN_STD 256
#define SEARCH_RESET -1

enum op_status {OP_ID = 0, ID, SET_VAL, PUT_VAL, TILE, SYM};

#define isNum(arg)(arg>='0' && arg<='9')
#define isSym(arg)(strcmp(arg, ";"))

//take txt file, turn to char*
char* ftostr(char *path);
//check that token is valid
int check_valid(char* op, char** trgt, int limit);
//check that token is int
void check_int(const char* num);
//Check sym.
void check_sym(const char* sym, int* op, const int op_prev, int* visits);

#endif
