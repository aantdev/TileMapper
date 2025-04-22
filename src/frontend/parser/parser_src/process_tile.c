// OPERATION
// ID
// SYM {
// ID (FIELD NAME)
// SYM :
// assuming FIELD `path`, then STRING_LITERAL otherwise INT
// SYM , 
// if found 4 tiles SYM } then SYM ; Otherwise back to ID (FIELD NAME)

// #include "parser.h"
//
// #include <stdlib.h>
//
// void process_tile(parser* parser, tile tile, int fields_found, int current_field);
//
// void add_tile(parser* parser, tile tile);
// void set_path(tile* tile, char* path);
// void set_width(tile* tile, int width);
// void set_height(tile* tile, int height);
//
// void process_tile(parser* parser, tile new_tile, int fields_found, int current_field) {
//     token token_current = parser->tokens[parser->token_current];
//     if (parser->rstate == IDENTIFIER) {
//         if (token_current.type != TOKEN_IDENTIFIER)
//             exit(EXIT_FAILURE);
//         
//         new_tile.identifier = token_current.literal;
//         int fields_found = 0;
//         int current_field = -1;
//         
//         process_tile(parser, new_tile, fields_found, current_field);
//         return;
//     }    
//     
//     // if (parser->rstate == ) {
//     // 
//     // }
//     
// }
//
