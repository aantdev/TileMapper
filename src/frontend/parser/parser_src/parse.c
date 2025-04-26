#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "lexer.h"
#include "parser.h"

//TODO: SYNTAX CHECK (done for SET)
//TODO: PROCESS TILE
//TODO: PROCESS PUT

void report(token* token, char* path, char* message) {
    error_t descriptor = {
        .line = token->line, 
        .col = token->column,
        .message = message,
        .file_path = path,
    };
    error_submit(descriptor, false);
}

void recover(parser* p) {
    token* current = at(p->token_v, p->token_current);
    while ( current->type != TOKEN_OPERATOR && current->type != TOKEN_EOF) {
        p->token_current++;
        current = at(p->token_v, p->token_current);
    }    

    p->rstate = OPERATOR;
}

void parse(parser *parser) {
    const int operator_count = 3;
    const char* operators[] = {
        "set",
        "put",
        "tile"
    };
    
    token* token_current = at(parser->token_v, parser->token_current); 
    while ( token_current->type != TOKEN_EOF) {

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
                report(token_current, parser->file_path, "Expected valid operator");
                recover(parser);
                break;
        }


        token_current = at(parser->token_v, parser->token_current); 
    }
}
