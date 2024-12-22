#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char* ftostr(char *path){
    //try to open file
    FILE* fptr;
    if ((fptr = fopen(path, "r")) == NULL){
        perror("Error: ");
        return NULL;
    }
    
    //allocate space, write contents to space.
    char c;
    size_t max = LEN_STD;
    size_t count = 0;
    char* final = malloc(sizeof(char) * max);
    if (final == NULL){
        fclose(fptr);
        free(final);
        return NULL;
    }
    while ((c=fgetc(fptr)) != EOF) {
        if (count >= max){
            max *= 2;
            final = realloc(final, max);
            if (final == NULL){
                fclose(fptr);
                free(final);
                return NULL;
            }
        }
        final[count] = (char) c;
        count++;
    }
    
    //final realloc.
    final = realloc(final, count + 1);
    if (final == NULL){
        fclose(fptr);
        free(final);
        return NULL;
    }
    
    if (count > 0)
        final[count] = '\0';

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
