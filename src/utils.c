#include "../include/utils.h"
#define ARENA_IMPLEMENTATION

char* ftostr(char *path){
    //try to open file
    FILE* fptr;
    if ((fptr = fopen(path, "r")) == NULL){
        perror("Error: ");
        fclose(fptr);
        exit(1);
    }
    
    //allocate space, write contents to space.
    char c;
    size_t count = 0;
    char* final = malloc(sizeof(char) * LEN_STD);
    while ((c=fgetc(fptr)) != EOF) {
        if (count >= LEN_STD){
            final = realloc(final, LEN_STD*count);
        }
        final[count] = (char) c;
        count++;
    }
    
    //final realloc.
    final = realloc(final, count);
    final[count-1] = '\0';

    fclose(fptr);
    return final;
}

FILE* gen_csv_zero(map_data* map_struct){ 
    FILE* csv = fopen("LEVEL_CSV_BLANK.txt", "w");

    for (int i = 0; i < map_struct->dim_vars[1]; i++) {
        for (int j = 0; j < map_struct->dim_vars[0]; j++) {
            fprintf(csv, "0,"); // print col 0
        }
        
        fprintf(csv, "\n"); // print newline, advance to next y
    } 

    return csv;
}

void init_map(map_data* map){
    map->max_tiles = 5;
    map->max_pos = 25;

    map->tile_count = 0;
    map->pos_count = 0;
     
    map->tiles = arena_alloc(&GLOBL_ARENA, map->max_tiles * sizeof(tile));
    map->positions = arena_alloc(&GLOBL_ARENA, map->max_pos * sizeof(pos));
}

void add_tile(map_data *map, tile new_tile) {
    map->tiles[map->tile_count - 1] = new_tile;
    map->tile_count++;

    if (map->tile_count > map->max_tiles) 
        map->max_tiles += 5;
}

void alter_csv(map_data* map_pack){
    
    fclose(map_pack->csv);
}

int check_valid(char* op, char** trgt, int limit){
    int i;
    for (i = 0; i<limit; i++){
        if (strcmp(op, trgt[i]) == 0) 
            break; 
    }
    
    if (i == limit){
        i = -1;
    }
    return i;
}

void check_int(const char* num){
    size_t i;
    for(i = 0; i < strlen(num); i++){
        if (!(num[i] >= '0' && num[i] <= '9')) {
            printf("Expected int, got %c.\n", num[i]); 
            exit(1);
        }
    }
}

void check_sym(const char *sym, int* op, int* op_prev, int* visits, int active_op){
    const char symbol = sym[0];
    printf("%d::%d::%d::%c\n", *visits, active_op, *op_prev, symbol);
    if (active_op == SET_VAL || *op_prev == *op) { 
        if (symbol != ';') {
            printf("Expecting ';'\n");
            exit(-1);
        }
         
        printf("--done--\n");
        *visits = 0;
        *op = OP_ID;
        return;
    }
    
    //TODO: Simplify PUT
    if (active_op == PUT_VAL){ // PUT
        if (*visits == 0){
            if (symbol != '('){
                printf("Expecting (");
                exit(-1);
            }

            *op = PUT_VAL;
            return;
        }

        if (*visits == 4){
            if(symbol != ')') { 
                printf("Expecting )");
                exit(-1);
            }
            *op = SYM;
            return;
        }    

        if (*visits%2 != 0){
            if (symbol != ',') { 
                printf("Expecting ,");
                exit(-1);
            }
            *op = PUT_VAL;
            return;
        }else {
            if (symbol == ','){
                *op = PUT_VAL;
                return;
            }

            if (symbol == ')'){
                printf("Back to sym\n");
                *op = SYM;
                return;
            }
            
            printf("Expecting , or ), got: %c", symbol);
            exit(-1);
        }
    }

    if (active_op == TILE){
        if (*visits == 1 ) {
            if (symbol == '{') { *op = ID; return;}
            if (symbol == ':') { *op = TILE_VAL; return;}
            exit(1); // some invalid symbol
        }

        if (*visits%2 == 0) {
            if ( *op_prev == ID ) {
                if (symbol == ':') { *op = TILE_VAL; return; }
                if (symbol == ',') { *op = ID; return; }

                printf("Unexpected symbol %c\n", symbol);
                exit(1);
            }

            if ( *op_prev == TILE_VAL ) {
                if (symbol == ',') { *op = ID; return; }
                printf("Unexpected symbol %c\n", symbol);
                exit(1);
            }
        }

        if (*visits == 3) {
            if ( *op_prev == ID ) {
                if (symbol == ':') { *op = TILE_VAL; return; }
                if (symbol == ',') { *op = ID; return; }

                printf("Unexpected symbol %c\n", symbol);
                exit(1);
            }

            if ( *op_prev == TILE_VAL ) {
                if (symbol == ',') { *op = ID; return; }
                if (symbol == '}') { *op = SYM; return; } 
                printf("Unexpected symbol %c\n", symbol);
                exit(1);
            }
        }

        if (*visits == 5 ) {
            if (*op_prev == ID) {
                if (symbol == ':') {*op = TILE_VAL; return; } 
            }
            if ( symbol == '}' ) {*op = SYM; return;}
            exit(1);
        }

    }
}

