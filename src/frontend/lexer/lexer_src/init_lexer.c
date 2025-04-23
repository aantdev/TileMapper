#include <string.h>
#include <stdio.h>

#include "error.h"
#include "lexer.h"
#include "file_string.h"

void init_lexer(lexer* lexer, char* path) {
    
    lexer->src_path = path;
    lexer->src = file_read(path);
    lexer->current = 0;
    lexer->line = 0;
    lexer->column = 0;

    // check if file_to_string succeeded first.
    error_if(lexer->src);
    lexer->length = strlen(lexer->src);
    
    // init internal token vector
    lexer->token_v = init_vector();
    error_if(lexer->token_v);
} 
