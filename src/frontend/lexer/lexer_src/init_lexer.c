#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "file_string.h"

void init_lexer(lexer* lexer, char* path) {
    
    lexer->src_path = path;
    lexer->src = file_read(path);
    lexer->current = 0;
    lexer->line = 0;
    lexer->column = 0;

    // check if file_to_string succeeded first.
    if (lexer->src == NULL){
        fprintf(stderr, "Failed to allocate source string!\n");
        lexer->flag = INIT_FAILURE;
        return;
    }
    lexer->length = strlen(lexer->src);
    
    // init internal token vector
    lexer->token_v = init_vector();
    if (lexer->token_v == NULL) {
        fprintf(stderr, "Failed to init internal token Vector!\n");
        lexer->flag = INIT_FAILURE;
        return;
    }

    lexer->flag = SUCCESS_RUNNING;
} 
