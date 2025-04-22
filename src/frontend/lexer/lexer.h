#ifndef LEXER_H 
#define LEXER_H

#include "vector.h"

typedef enum {
    SUCCESS_RUNNING,
    SUCCESS_DONE,
    INIT_FAILURE,
    ADD_TOKEN_FAILURE,
    UNSUPORTED_CHAR,
    INVALID_STRING_LITERAL
} err_code;

typedef enum {
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_STRING_LITERAL,
    TOKEN_NUMBER,
    TOKEN_EOF,
    TOKEN_RBRACKET_LEFT  = '(',
    TOKEN_RBRACKET_RIGHT = ')',
    TOKEN_BRACKET_LEFT   = '{',
    TOKEN_BRACKET_RIGHT  = '}',
    TOKEN_SEMICOLON      = ';',
    TOKEN_COLON          = ':', 
    TOKEN_COMMA          = ',',
} t_type;

typedef struct {
    t_type type;
    char* literal;
    int line;
    int column;
} token;

typedef struct {
    char* src_path;
    char* src;
    int length;
    int current;
    int line;
    int column;
    
    vector_t* token_v;
    
    err_code flag;
} lexer;

void init_lexer(lexer* lex, char* path);
void close_lexer(lexer* lex);

void add_token(lexer* lex, int start, int length, t_type type);
void tokenize(lexer* lex);

#endif
