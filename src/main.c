#include <stdio.h>
#include <stdlib.h>

#define VECTOR_IMPLEMENTATION
#include "lexer.h"
#include "parser.h"
#include "error.h"

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

    parser parser = {0};
    init_parser(&parser, &lex);
    parse(&parser);
    
    printf("\nX = %d\n", parser.dim_vars[0]);
    printf("Y = %d\n", parser.dim_vars[1]);
    printf("Tile_size = %d\n", parser.dim_vars[2]);

    return 0;
}
