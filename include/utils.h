#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEN_STD 256
#define SEARCH_RESET -1

#define DIM_COUNT 3
// enum dim_vars  {X = 0, Y = 1, TS = 2};

enum op_status {OP_ID = 0, ID, SET_VAL, PUT_VAL, TILE, SYM, TILE_VAL};

#define isNum(arg)(arg>='0' && arg<='9')
#define isSym(arg)(strcmp(arg, ";"))

typedef struct {
    //user given info
    char* texture_path;
    int animated;
    int width;
    int height;
    
    // info only in json
    char* name; // this is given by the user
    int id;
}tile;

typedef struct {
    int coord_count;
    int* coords;

    int tile_id;
}pos;

typedef struct { 
    int dim_vars[DIM_COUNT];

    tile* tiles;    // Turn these into json files 
    pos* positions; // This we need for the csv
}map_data;

char* ftostr(char *path); //take txt file, turn to char*
int check_valid(char* op, char** trgt, int limit); //check that token is valid
void check_int(const char* num); //check that token is int
void check_sym(const char* sym, int* op, int* op_prev, int* visits, int active_op); //Check sym.

void pos_to_csv(map_data data);     //TODO
void tile_to_json(map_data data);   //TODO

#endif
