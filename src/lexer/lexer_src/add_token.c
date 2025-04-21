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

/*
void add_token(lexer *lex, int start, int length, t_type type) {

    if (type == TOKEN_EOF) {
        lex->token_count++;
        lex->tokens = realloc(lex->tokens, lex->token_count * sizeof(token));
        if (lex->tokens == NULL) {
            fprintf(stderr, "Realloc failed in add_token!\n");
            lex->flag = ADD_TOKEN_FAILURE;
            return;
        }

        lex->tokens[lex->token_count-1].type = TOKEN_EOF;
        lex->tokens[lex->token_count-1].literal = NULL;
        lex->tokens[lex->token_count-1].line = lex->line;
        lex->tokens[lex->token_count-1].column = lex->column;
        lex->flag = SUCCESS_RUNNING;
        return;
    }

    lex->tokens[lex->token_count].literal = malloc(length);
    if (lex->tokens[lex->token_count].literal == NULL) {
        lex->flag = ADD_TOKEN_FAILURE;
        fprintf(stderr, "Failed to add token!\n");
        return;
    }
    memcpy(lex->tokens[lex->token_count].literal, lex->src + start, length-1); 
    lex->tokens[lex->token_count].literal[length-1] = '\0';
    
    lex->tokens[lex->token_count].line = lex->line;
    lex->tokens[lex->token_count].column = lex->column-(length-1);
    lex->tokens[lex->token_count].type = type;
    
    lex->token_count++;
    if (lex->token_count > lex->token_limit - 1){
        lex->token_limit *= 2;
        lex->tokens = realloc(lex->tokens, sizeof(token) * lex->token_limit);
        if (lex->tokens == NULL) {
            fprintf(stderr, "Realloc failed after reaching max tokens!\n");
            lex->flag = ADD_TOKEN_FAILURE;
            return;
        }
    }

    lex->flag = SUCCESS_RUNNING;
}
*/
