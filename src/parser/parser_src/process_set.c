#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"

void process_set(parser *parser, int set_var){
    token* token_current = at(parser->token_v, parser->token_current);
    if (parser->rstate == IDENTIFIER) { 
        if (token_current->type != TOKEN_IDENTIFIER){
            exit(EXIT_FAILURE);
        }
        
        int set_variable = -1;
        for (int i = 0; i < parser->dim_count; i++) {
            if (strcmp(token_current->literal, parser->set_array[i]) == 0) {
                set_variable = i;
                break;
            }
        }

        if (set_variable == -1) {
            exit(EXIT_FAILURE);
        }

        parser->rstate = NUMBER;
        parser->token_current++;
        process_set(parser, set_variable);

        return;
    } 

    if (parser->rstate == NUMBER) {
        if (token_current->type != TOKEN_NUMBER) {
            exit(EXIT_FAILURE);
        }

        int temp_int;
        if (!sscanf(token_current->literal, "%d", &temp_int))
            exit(EXIT_FAILURE);

        parser->dim_vars[set_var] = temp_int;       

        parser->rstate = SYMBOL_SEMICOLON;
        parser->token_current++;
        process_set(parser, set_var);

        return;
    }

    if (parser->rstate == SYMBOL_SEMICOLON) {
        if (token_current->type != TOKEN_SEMICOLON) {
            exit(EXIT_FAILURE);
        }

        parser->rstate = OPERATOR;
        parser->token_current++;
        parser->active_op = NONE;
        return;
    }
}
