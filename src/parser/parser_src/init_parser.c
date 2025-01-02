#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

void init_parser(parser *parser, lexer *lex) {
    parser->tokens = lex->tokens;
    parser->token_count = lex->token_count;
    parser->token_current = 0;
    parser->file_path = lex->src_path;
    
    // free string from lex
    free(lex->src);

    // Expect operator
    parser->rstate = OPERATOR;
    parser->active_op = NONE;

    //three variables: X,Y,TILE_SIZE
    parser->dim_count = 3;
    parser->set_array[0] = "X";
    parser->set_array[1] = "Y";
    parser->set_array[2] = "TS";
    
    parser->tile_count = 0;
    parser->max_tiles = MAX_TILES;
    parser->tiles = malloc(MAX_TILES * sizeof(tile));
    if (parser->tiles == NULL) {
        printf("Parser init failed\n");
        exit(EXIT_FAILURE);
    }
}
