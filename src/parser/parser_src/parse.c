#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

//TODO: SYNTAX CHECK
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
