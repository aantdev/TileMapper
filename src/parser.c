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

//TODO: SYNTAX CHECK
//TODO: PROCESS SET
//TODO: PROCESS TILE
//TODO: PROCESS PUT
void parse(parser *parser) {
    const int operator_count = 3;
    const char* operators[] = {
        "set",
        "put",
        "tile"
    };

    int set_variable;

    while (parser->tokens[parser->token_current].type != TOKEN_EOF) {
        token token_current = parser->tokens[parser->token_current]; 
        if (parser->rstate == OPERATOR) {
            //find operator in accepted operators
            int i;
            for (i = 0; i < operator_count; i++) {
                if (strcmp(token_current.literal, operators[i]) == 0) {
                    // 0 = SET, 1 = PUT, 2 = TILE
                    parser->active_op = i;
                    break;
                }
            }

            if (i == 3) {
                printf("Expected valid operator! %s\n", token_current.literal);
                exit(EXIT_FAILURE); 
            }
            
            parser->rstate = IDENTIFIER;
            parser->token_current++;
            continue;
        }

        if (parser->active_op == SET && parser->rstate == IDENTIFIER) { 
            if (token_current.type != TOKEN_IDENTIFIER){
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < parser->dim_count; i++) {
                if (strcmp(token_current.literal, parser->set_array[i]) == 0) {
                    set_variable = i;
                    break;
                }
            }

            parser->rstate = NUMBER;
            parser->token_current++;
            continue;
        } 

        if (parser->active_op == SET && parser->rstate == NUMBER) {
            if (token_current.type != TOKEN_NUMBER) {
                exit(EXIT_FAILURE);
            }

            int temp_int;
            if (!sscanf(token_current.literal, "%d", &temp_int))
                exit(EXIT_FAILURE);

            parser->dim_vars[set_variable] = temp_int;       

            parser->rstate = SYMBOL;
            parser->token_current++;
            continue;
        }

        if (parser->active_op == SET && parser->rstate == SYMBOL) {
            if (token_current.type != TOKEN_SYMBOL || token_current.literal[0] != ';') {
                exit(EXIT_FAILURE);
            }

            parser->rstate = OPERATOR;
            parser->token_current++;
            parser->active_op = NONE;
        }

    }
    exit(EXIT_SUCCESS);
}
