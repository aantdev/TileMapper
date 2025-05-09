#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#define MAX_TILES 10

typedef enum {
    OPERATOR,
    IDENTIFIER,
    FIELD,
    NUMBER,
    SYMBOL,
    SYMBOL_SEMICOLON,
    SYMBOL_COLON,
    SYMBOL_COMMA,
    SYMBOL_LBRACKET,
    SYMBOL_RBRACKET,
    SYMBOL_LRBRACKET,
    SYMBOL_RRBRACKET
} read_state;

typedef enum {
    SET,
    PUT,
    TILE,
    NONE
} active_op;

typedef enum {
    X,
    Y,
    TS
} dim_enum;

typedef struct {
    char* identifier;
    int id;

    char* texture;
    int animated;
    int width;
    int height;
} tile;

typedef struct {
    // lexer data(tokens)
    // Tokens terminated with EOF Token
    vector_t* token_v;
    int token_current;
    char* file_path;

    // read state and active operation
    read_state rstate;
    active_op active_op;
    
    vector_t* tile_v;
    char* accepted_fields[4];

    char* set_array[3];
    int dim_vars[3];
    int dim_count;
} parser;

void report(token* token, char* path, char* message);

void init_parser(parser* parser, lexer* lex);
void parse(parser* parser);

void process_set(parser* parser, int set_var);
// void resolve_put(parser* parser, token token_current);
// void resolve_tile(parser* parser, token token_current);

#endif
