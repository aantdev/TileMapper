#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "utils.h"

void init_lexer(lexer* lexer, char* path) {
    lexer->src_path = path;
    lexer->src = ftostr(path);
    lexer->current = 0;
    lexer->line = 0;
    lexer->column = 0;
    lexer->token_count = 0;
    lexer->token_limit = MAX_TOKENS;

    // check if file_to_string succeeded first.
    if (lexer->src == NULL){
        fprintf(stderr, "Failed to allocate source string!\n");
        lexer->flag = INIT_FAILURE;
        return;
    }
    lexer->length = strlen(lexer->src);
    
    // if all is ok, then alloc some tokens
    lexer->tokens = malloc(sizeof(token) * MAX_TOKENS);
    if (lexer->tokens == NULL) {
        fprintf(stderr, "Failed to allocate tokens!\n");
        lexer->flag = INIT_FAILURE;
        return;
    }

    lexer->flag = SUCCESS_RUNNING;
} 

void close_lexer(lexer *lex) {
    if (lex->tokens != NULL) {
        for (int i = 0; i < lex->token_count; i++) {
            if (lex->tokens[i].literal != NULL && lex->tokens[i].type != TOKEN_EOF){
                free(lex->tokens[i].literal);
            }
        }

        free(lex->tokens);
    } 
    
    if (lex->src != NULL)
        free(lex->src);

    lex->src = NULL;
    lex->tokens = NULL;
}

void add_token(lexer *lex, int start, int length, t_type type) {

    if (type == TOKEN_EOF) {
        lex->token_count++;
        lex->tokens = realloc(lex->tokens, lex->token_count * sizeof(token));
        if (lex->tokens == NULL) {
            fprintf(stderr, "Realloc failed in add_token!\n");
            lex->flag = ADD_TOKEN_FAILURE;
            return;
        }

        lex->tokens[lex->token_count-1].type = TOKEN_EOF;
        lex->tokens[lex->token_count-1].literal = NULL;
        lex->tokens[lex->token_count-1].line = lex->line;
        lex->tokens[lex->token_count-1].column = lex->column;
        lex->flag = SUCCESS_RUNNING;
        return;
    }

    lex->tokens[lex->token_count].literal = malloc(length);
    if (lex->tokens[lex->token_count].literal == NULL) {
        lex->flag = ADD_TOKEN_FAILURE;
        fprintf(stderr, "Failed to add token!\n");
        return;
    }
    memcpy(lex->tokens[lex->token_count].literal, lex->src + start, length-1); 
    lex->tokens[lex->token_count].literal[length-1] = '\0';
    
    lex->tokens[lex->token_count].line = lex->line;
    lex->tokens[lex->token_count].column = lex->column-(length-1);
    lex->tokens[lex->token_count].type = type;
    
    lex->token_count++;
    if (lex->token_count > lex->token_limit - 1){
        lex->token_limit *= 2;
        lex->tokens = realloc(lex->tokens, sizeof(token) * lex->token_limit);
        if (lex->tokens == NULL) {
            fprintf(stderr, "Realloc failed after reaching max tokens!\n");
            lex->flag = ADD_TOKEN_FAILURE;
            return;
        }
    }

    lex->flag = SUCCESS_RUNNING;
}

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
        int newline = (c == '\n');
        int d_quote = (c == '"');
        int extra_char = (c == '_');
        const int symbol = ( c == ';' ) || ( c == '=' ) || 
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
            add_token(lex, start, length, TOKEN_SYMBOL);

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
