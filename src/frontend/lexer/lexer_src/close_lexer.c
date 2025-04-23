#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "error.h"
#include "lexer.h"

// close lexer called on termination with errors

void destroy_token(token* token) {
    free(token->literal);
    token->literal = NULL;
}

void close_lexer(lexer *lex) {
    
    for (size_t i=0; i < lex->token_v->back; i++) {
        destroy_token(at(lex->token_v, i));
    }
    free_vector(lex->token_v);
    lex->token_v = NULL;

    free(lex->src);
}

