#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "utils.h"

void init_lexer(lexer* lexer, char* path) {
    lexer->src_path = path;
    lexer->src = ftostr(path);
    lexer->current = 0;
    lexer->line = 0;
    lexer->column = 0;
    lexer->token_count = 0;
    lexer->token_limit = MAX_TOKENS;

    // check if file_to_string succeeded first.
    if (lexer->src == NULL){
        fprintf(stderr, "Failed to allocate source string!\n");
        lexer->flag = INIT_FAILURE;
        return;
    }
    lexer->length = strlen(lexer->src);
    
    // if all is ok, then alloc some tokens
    lexer->tokens = malloc(sizeof(token) * MAX_TOKENS);
    if (lexer->tokens == NULL) {
        fprintf(stderr, "Failed to allocate tokens!\n");
        lexer->flag = INIT_FAILURE;
        return;
    }

    lexer->flag = SUCCESS_RUNNING;
} 
