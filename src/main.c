#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"
#include "utils.h"

char* temp[] = {"Keyword", "Number", "Symbol", "EOF"};
int main(int argc, char** argv) {
    lexer lex = {0};
    if (argc != 2) {
        fprintf(stderr, "Usage:\n./lex <input.txt>\n");
        exit(EXIT_FAILURE);
    }
    
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

    parser parser = {0};
    init_parser(&parser, &lex);
    parse(&parser);

    return 0;
}
