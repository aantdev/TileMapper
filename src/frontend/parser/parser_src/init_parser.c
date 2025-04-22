#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

void init_parser(parser *parser, lexer *lex) {
    parser->token_v = lex->token_v;

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

    // parser->accepted_fields[0] = "texture_path";
    // parser->accepted_fields[1] = "animated";
    // parser->accepted_fields[2] = "width";
    // parser->accepted_fields[3] = "height";

    parser->tile_v = init_vector();
    if (parser->tile_v == NULL) {
        fprintf(stderr, "Could not init internal tile vector!\n");
    }
}
