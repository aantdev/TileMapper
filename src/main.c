#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "utils.h"

char* temp[] = {"Keyword", "Number", "Symbol", "EOF"};
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage:\n./lex <input.txt>\n");
        exit(EXIT_FAILURE);
    }
    
    lexer lex = {0};
    init_lexer(&lex, argv[argc-1]);
    error_handle(&lex);

    tokenize(&lex);
    error_handle(&lex);
    for (int i = 0; i < lex.token_count; i++) {
        printf("Token %s at %d::%d of type %d\n", 
                lex.tokens[i].literal,
                lex.tokens[i].line,
                lex.tokens[i].column,
                lex.tokens[i].type);
    }

    return 0;
}
