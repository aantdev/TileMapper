#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include "lexer.h"

typedef struct {
    size_t line;
    size_t col; 

    char* file_path;
    char* file_name;
    char* message;
} error_t;

void error_init(void);
void error_if(void* ptr);

error_t error_new(char* message, token token);
void error_submit(error_t error, bool fatal);

void error_final(void);
void success_close(void);

#endif
