#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#define MAX_TILES 10

typedef enum {
    OPERATOR,
    IDENTIFIER,
    NUMBER,
    SYMBOL    
} read_state;

typedef enum {
    SET,
    PUT,
    TILE
} active_op;

typedef struct {
    char* identifier;
    char* texture;
    int animated;
    int id;
} tile;

typedef struct {
    // lexer data(tokens)
    // Tokens terminated with EOF Token
    token* tokens;
    int token_count;

    // read state and active operation
    read_state rstate;
    active_op active_op;
    
    tile* tiles;
    int max_tiles;
    int tile_count;

    char* set_array[3];
    int dim_vars[3];
    int dim_count;
} parser;

void init_parser(parser* parser, lexer* lex);
void parse(parser* parser);

void resolve_set(char* identifier);

void check_tile_overlap(char* identifier);
void check_tile_exists(char* identifier);
#endif
