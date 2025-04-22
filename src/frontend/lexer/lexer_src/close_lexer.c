#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

void close_lexer(lexer *lex) {
    
    for (size_t i=0; i < lex->token_v->back; i++) {
        free(((token*)lex->token_v->data_array[i])->literal);
    }
    free_vector(lex->token_v);

}

