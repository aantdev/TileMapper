#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char* ftostr(char *path){
    //try to open file
    FILE* fptr;
    if ((fptr = fopen(path, "rb")) == NULL){
        perror("Error: ");
        return NULL;
    }
    char* final = NULL;

    // allocate space, write contents to space
    if (fseek(fptr, 0, SEEK_END) != 0) {
        exit(EXIT_FAILURE);
    }
    long len = ftell(fptr);
    if (len == -1) {
        exit(EXIT_FAILURE);
    }
    rewind(fptr);
    final = malloc(sizeof(char) * len+1);

    size_t shit_read = fread(final, sizeof(char), len, fptr);
    if (shit_read != (size_t)len) {
        printf("Shit hit the fan\n");
        exit(EXIT_FAILURE);
    }

    final[len] = '\0';

    fclose(fptr);
    return final;
}

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
