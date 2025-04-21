#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"


void add_token(lexer *lex, int start, int length, t_type type) {
    token new_token = {type, NULL, lex->line, lex->column};
    
    if (type == TOKEN_EOF)
        return;

    // allocate some memory for the string and memcpy
    new_token.literal = malloc(length * sizeof(char));
    if (new_token.literal == NULL) {
        fprintf(stderr, "Failed to allocated space for string literal");
        lex->flag = ADD_TOKEN_FAILURE;
        return;
    }
    memcpy(new_token.literal, lex->src+start, length);    
    new_token.literal[length-1] = '\0';

    push_back(lex->token_v, &new_token, sizeof(new_token));
    if (lex->token_v->data_array == NULL) {
        fprintf(stderr, "Could not expand token stream for push_back!\n");
        exit(EXIT_FAILURE);
    }
    lex->flag = SUCCESS_RUNNING;

}

