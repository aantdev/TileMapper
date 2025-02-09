#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

void close_lexer(lexer *lex) {
    if (lex->tokens != NULL) {
        for (int i = 0; i < lex->token_count; i++) {
            if (lex->tokens[i].literal != NULL && lex->tokens[i].type != TOKEN_EOF){
                free(lex->tokens[i].literal);
            }
        }

        free(lex->tokens);
    } 
    
    if (lex->src != NULL)
        free(lex->src);

    lex->src = NULL;
    lex->tokens = NULL;
}
