#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void error_handle(lexer* lex) {
    if (lex->flag == UNSUPORTED_CHAR) {
        fprintf(stderr, "%s:%d::%d:Syntax Error: Unsupported character\n", 
                lex->src_path, lex->line+1, lex->column+1);
        close_lexer(lex);
        exit(EXIT_FAILURE);
    }

    if (lex->flag == INIT_FAILURE) {
        fprintf(stderr, "Could not init Lexer!\n");
        close_lexer(lex);
        exit(EXIT_FAILURE);
    }

    if (lex->flag == ADD_TOKEN_FAILURE) {
        fprintf(stderr, "Could not add token!\n");
        close_lexer(lex);

        exit(EXIT_FAILURE);
    }

    if (lex->flag == SUCCESS_DONE) {
        close_lexer(lex);
        printf("Terminated with success.\n");
        return;
    }
    
    if (lex->flag == INVALID_STRING_LITERAL) {
        fprintf(stderr, "%s:%d::%d:Syntax Error: Invalid string literal\n", 
                lex->src_path, lex->line+1, lex->column+1);
        close_lexer(lex);
        exit(EXIT_FAILURE);
    }
}
