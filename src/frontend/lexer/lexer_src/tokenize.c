#include <string.h>
#include <ctype.h>

#include "lexer.h"

void tokenize(lexer* lex) {
    char* operators[] = {
        "set",
        "put",
        "tile"
    };
    int operator_count = 3;
    while (lex->current < lex->length) {
        char c = lex->src[lex->current]; 
        const char c_n = lex->src[lex->current+1]; 
        
        // TODO:tokenize string literal
        const int whitespace = (c == ' ');
        int newline = (c == '\n' | c == '\r');
        int d_quote = (c == '"');
        int extra_char = (c == '_');
        const int symbol = ( c == ';' ) ||  
                           ( c == ')' ) || ( c == '(' ) || 
                           ( c == '}' ) || ( c == '{' ) ||
                           ( c == ',' ) || ( c == ':' ) ;
        

        const int comment = (c == '/') && (c_n == '/');
        // Newline or whitespace, skip
        if (whitespace) {
            lex->current++;
            lex->column++;
            continue;
        }

        if (newline) {
            lex->current++;
            lex->column = 0;
            lex->line++;
        }

        // Caught comment, skip.
        if (comment) {
            while (lex->src[lex->current] != '\n') {
                lex->current++;
            }
            lex->column = 0;
            lex->line++;
            continue;
        }

        // Caught symbol. store as token.
        // TOKEN_SYMBOL
        if (symbol) {
            // exactly two chars. [0] = 'char', [1] = '\0'
            int length = 2;
            int start = lex->current;
            t_type type = c;

            add_token(lex, start, length, type);

            lex->current++;
            lex->column++;
            continue;
        }
        
        // Caught character, start reading.
        // then identify token type
        // then store as token
        if (isalpha(c) || extra_char ) {
            int start = lex->current;
            while (isalpha(c) || extra_char) {
                lex->current++;
                lex->column++;
                
                c = lex->src[lex->current];
                extra_char = (c == '_');
            }

            int length = (lex->current - start) + 1;
            
            t_type type = TOKEN_IDENTIFIER;
            for (int i = 0; i < operator_count; i++) {
                if (memcmp(lex->src + start, operators[i], length-1) == 0){
                    type = TOKEN_OPERATOR;
                }
            }

            add_token(lex, start, length, type);
            continue;
        }
    
        // caught digit
        // store as TOKEN_NUMBER
        if (isdigit(c)) {
            int start = lex->current;
            while (isdigit(c)) {
                lex->current++;
                lex->column++;

                c = lex->src[lex->current];
            }
            int length = (lex->current - start) + 1;
            
            add_token(lex, start, length, TOKEN_NUMBER);
            continue;
        }

        if (d_quote) {
            int start = lex->current;
            do {
                lex->current++;
                // string literal not terminated.
                if ((lex->current == lex->length) || newline) {
                    lex->flag = INVALID_STRING_LITERAL;
                    return;
                }
                lex->column++;
                               
                c = lex->src[lex->current];
                newline = ( c == '\n' ); 
                d_quote = ( c == '"' );
            } while (!d_quote);
            lex->current++;
            int length = (lex->current - start) + 1;
            
            add_token(lex, start, length, TOKEN_STRING_LITERAL);
            continue;
        }
    } 
    
    // terminate tokens with EOF token
    add_token(lex, 0, 0, TOKEN_EOF);
    lex->flag = SUCCESS_RUNNING;
}
