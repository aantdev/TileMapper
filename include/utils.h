#ifndef UTILS_H
#define UTILS_H

#define LEN_STD 256
#include "lexer.h"

char* ftostr(char *path); //take txt file, turn to char*
void error_handle(lexer* lex);

#endif
