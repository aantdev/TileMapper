#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "parser.h"

//TODO: SYNTAX CHECK (done for SET)
//TODO: PROCESS TILE
//TODO: PROCESS PUT
void parse(parser *parser) {
    const int operator_count = 3;
    const char* operators[] = {
        "set",
        "put",
        "tile"
    };
    

    while ( ((token*)at(parser->token_v, parser->token_current))->type != TOKEN_EOF) {
        token* token_current = at(parser->token_v, parser->token_current); 

        // first step, expecting valid operator
        if (parser->rstate == OPERATOR) {
            //find operator in accepted operators
            int i;
            for (i = 0; i < operator_count; i++) {
                if (strcmp(token_current->literal, operators[i]) == 0) {
                    // 0 = SET, 1 = PUT, 2 = TILE
                    parser->active_op = i;
                    break;
                }
            }

            if (i == 3) {
                error_t descriptor = {
                    .line = token_current->line, 
                    .col = token_current->column,
                    .message = "Expected a valid operator",
                    .file_path = parser->file_path,
                };
                error_submit(descriptor, false);
            }
            
            parser->rstate = IDENTIFIER;
            parser->token_current++;

            // change cycle
            continue;
        }
        
        // process operation
        switch (parser->active_op) {
            case SET:
                process_set(parser, -1); 
                break;
            case PUT:
                // process_put(parser);
                break;
            case TILE:
                // process_tile(parser);
                break;
            case NONE:
                exit(EXIT_FAILURE);
        }


    }
}
