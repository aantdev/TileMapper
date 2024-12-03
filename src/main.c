#define ARENA_IMPLEMENTATION
#include "../include/utils.h"

//TODO: Fix error handling.
int valid_opc = 3;
char* valid_ops[] = {
    "set",
    "put",
    "tile"
};

int valid_idc = 3;
char* valid_id[] = {
    "X",
    "Y",
    "TS"
};

int valid_putc = 2; 
char* valid_id_put[] = {
    "C",
    "P"
};

int valid_fieldc = 4;
int found_fields[] = {0,0,0,0};
char* valid_field[] = {
    "texture",
    "animated",
    "width",
    "height"
};

map_data map_pack;
void parse(char* str);

int main(int argc, char** argv){
    if (argc != 2){
        printf("Error: Invalid number of arguments.\nUsage: mapper in.txt\n");
        exit(1);
    }

    init_map(&map_pack);
        
    char* fstring = ftostr(argv[argc-1]); //copy full file to string
    parse(fstring);
    
    
    printf("X: %d, Y: %d, TS: %d\n", 
            map_pack.dim_vars[0], map_pack.dim_vars[1], map_pack.dim_vars[TS]);
    

    free(fstring);
    arena_free(&GLOBL_ARENA);
    return 0;
}

void parse(char* str){
    int begin_op = -1;
    int prev_op = -1;
    int active_op = -1;
    int op_id = OP_ID;

    int visitc = 0;

    int first = SEARCH_RESET;
    int last = SEARCH_RESET;
    
    int fl = -1;
    int after_char = 0;
    
    int id_index;
    char tmp[256];
    
    int i = 0;
    char c = str[i];
    while (c!='\0') {
        c = str[i];
        const char c_next = str[i+1];
        
        // skip comment 
        if (c == '/' && c == c_next) {
            //skip entire line.
            while (c!='\n' && c!='\000') {
                i++;
                c = str[i];
            }
        }

        const int white_or_new = (c == ' ') || (c == '\n');
        const int symbol = ( c == ';' ) || ( c == '=' ) || 
                           ( c == ')' ) || ( c == '(' ) || 
                           ( c == '}' ) || ( c == '{' ) ||
                           ( c == ',' ) || ( c == ':' ) ;

        if (!white_or_new && first == SEARCH_RESET) { 
            after_char = 1; 
            first = i; 
        }

        if (white_or_new && !after_char) { i++; c = str[i]; continue; } 
        if (after_char && (white_or_new || symbol)) {last = i; i--;}
        
        if (first != SEARCH_RESET && last != SEARCH_RESET){
            //copy to temp char* and null terminate.
            const int n_offset = (last-first) ? last-first : (last-first) + 1;
            memcpy(tmp, str+first, (last-first)+1);
            tmp[n_offset] = '\0';
            
            begin_op = op_id; 
            //SYM, or ; is the last search state. Process, and reset
            if (op_id == SYM) {
                check_sym(tmp, &op_id, &prev_op, &visitc, active_op);

                prev_op = begin_op;
                i+=2; //currently i points to last digit before ;. advance twice.
                after_char = 0;
                first = SEARCH_RESET;
                last = SEARCH_RESET;
                continue;
            }

            // Search for the set value. Expect int.
            if (op_id == SET_VAL){
                check_int(tmp);
                map_pack.dim_vars[id_index] = atoi(tmp); 

                op_id=SYM;
            }
            
            //searching for put value.
            if (op_id == PUT_VAL){
                check_int(tmp);
                //placeholder, write placement data to struct.
                op_id=SYM; 
                visitc++;
            }
            
            if (op_id == TILE_VAL) {
                switch (id_index) {
                    case 0:
                        
                    break;
                    case 1:

                    break;
                    case 2:

                    break;
                    case 3:

                    break;
                }

                op_id = SYM; //TODO: ACTUALLY CHECK VALIDITY
            }

            // Search for target identifier
            if (op_id == ID) {
                op_id += fl+1;
                
                switch (op_id) {
                    //Does the id exist? 
                    //Then proceed.
                    case SET_VAL:
                        id_index = check_valid(tmp, valid_id, valid_idc);                        
                    break;
                    
                    //check if identifier exists.
                    //move to sym if yes.
                    case PUT_VAL:
                        id_index = check_valid(tmp, valid_id_put, valid_putc);
                        op_id = SYM;
                    break;
                    
                    // Move to sym. 
                    case TILE:
                        // place tile name in valid tile list
                        if (visitc == 0) {
                            visitc++;
                            op_id = SYM;
                            // add tile name to valid_put 

                            break;
                        }
                        id_index = check_valid(tmp, valid_field, valid_fieldc);
                        visitc++;
                        op_id = SYM;
                    break;
                }
                if (id_index == -1) { 
                    printf("Invalid identifier '%s,%d'.\n", tmp,id_index); 
                    exit(1); 
                }

            }

            //search for operation identifier
            if (op_id == OP_ID) {
                fl = check_valid(tmp, valid_ops, valid_opc); 
                if( fl == valid_opc) { 
                    printf("OP_ID: Invalid identifier '%s'.", tmp); 
                    exit(1);
                }
                
                active_op = fl+2;
                printf("Active: %d\n", active_op);

                op_id = ID; 
            }

            //token processed, reset for next token.
            prev_op = begin_op;
            after_char = 0;
            first = SEARCH_RESET;
            last = SEARCH_RESET;
            printf("End cycle: %d, %d\n", prev_op, op_id);
        }
        i++;
    }
}
