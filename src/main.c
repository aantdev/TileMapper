#include <stdio.h>
#include <stdlib.h>

#define VECTOR_IMPLEMENTATION
#include "lexer.h"
#include "parser.h"
#include "error.h"

int main(int argc, char** argv) {
    lexer lex = {0};
    if (argc != 2) {
        fprintf(stderr, "Usage:\n./lex <input.txt>\n");
        exit(EXIT_FAILURE);
    }

    error_init();
    init_lexer(&lex, argv[argc-1]);
    tokenize(&lex);

    parser parser = {0};
    init_parser(&parser, &lex);
    parse(&parser);
    
    error_final();
}
