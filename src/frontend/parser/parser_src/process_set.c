#include "parser.h"

void process_set(parser *parser, int set_var){
    token* token_current = at(parser->token_v, parser->token_current);
    if (parser->rstate == IDENTIFIER) { 
        if (token_current->type != TOKEN_IDENTIFIER){
            report(token_current, parser->file_path, "Expected identifier");
        }
        
        parser->rstate = NUMBER;
        parser->token_current++;
        process_set(parser, -1);

        return;
    } 

    if (parser->rstate == NUMBER) {
        if (token_current->type != TOKEN_NUMBER) {
            report(token_current, parser->file_path, "Expected identifier or number");
        }

        parser->rstate = SYMBOL_SEMICOLON;
        parser->token_current++;
        process_set(parser, set_var);

        return;
    }

    if (parser->rstate == SYMBOL_SEMICOLON) {
        parser->rstate = OPERATOR;
        parser->active_op = NONE;
        if (token_current->type != TOKEN_SEMICOLON) {
            report(token_current, parser->file_path, "Expected symbol ;");
            return;
        }

        parser->token_current++;
        return;
    }
}
