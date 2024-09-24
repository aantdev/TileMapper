#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

enum id_set {WIDTH = 0, HEIGHT, TILE_SIZE};

int valid_opc = 3;
char* valid_ops[] = {
    "set",
    "put",
    "tile"
};  

int valid_idc = 2;
char* valid_id[] = {
    "X",
    "Y" 
};

int valid_putc = 2; 
char* valid_id_put[] = {
    "C",
    "P"
};

// width height info
int dim_vars[2] = {0, 0};

//store new entity identifiers here
char** entities;

void parse(char* str);

int main(int argc, char** argv){
    if (argc != 2){
        printf("Error: Invalid number of arguments.\nUsage: mapper in.txt\n");
        exit(1);
    }
    
    char* fstring = ftostr(argv[argc-1]); //copy full file to string
    printf("%s\n\n", fstring);
    parse(fstring);
        
    printf("X: %d, Y: %d\n", dim_vars[0], dim_vars[1]);

    free(fstring);
    return 0;
}


void parse(char* str){
    int op_id = OP_ID;
    int visitc = 0;

    int first = SEARCH_RESET;
    int last = SEARCH_RESET;
    
    int fl = -1;
    int after_char = 0;
    int white_or_new = 0;
    int symbol = 0;
    
    int id_index;
    int j = 0;
    char tmp[3][32];
    
    int i = 0;
    char c = str[i];
    char c_next;
    while (c!='\0') {
        c = str[i];
        c_next = str[i+1];
        
        if (c == '/' && c == c_next) {
            while (c!='\n' && c!='\000') {
                i++;
                c = str[i];
            }
        }

        white_or_new = (c == ' ') || (c == '\n');
        symbol = ( c == ';' ) || ( c == '=' ) || 
                 ( c == ')' ) || ( c == '(' ) || 
                 ( c == '}' ) || ( c == '{' );

        if (!white_or_new && first == SEARCH_RESET) { 
            after_char = 1; 
            first = i; 
            // printf("--%d,%c--\n", i, str[i]);
        }

        if (white_or_new && !after_char) { i++; c = str[i]; continue; } 
        if (after_char && (white_or_new || symbol)) {last = i; i--;}
        
        if (first != SEARCH_RESET && last != SEARCH_RESET){
            //copy to temp char* and null terminate.
            int n_offset = (last-first) ? last-first : (last-first) + 1;
            memcpy(tmp[j], str+first, (last-first)+1);
            tmp[j][n_offset] = '\0';
            
            
            //SYM, or ; is the last search state. Process, and reset
            if (op_id == SYM) {
                check_sym(tmp[j], &op_id, fl+2, &visitc);

                j = 0;
                i+=2; //currently i points to last digit before ;. advance twice.
                after_char = 0;
                first = SEARCH_RESET;
                last = SEARCH_RESET;
                continue;
            }

            // Search for the set value. Expect int.
            if (op_id == SET_VAL){
                
                check_int(tmp[j]);
                dim_vars[id_index] = atoi(tmp[j]); 

                op_id=SYM;
                visitc++;
            }
            
            //searching for put value.
            if (op_id == PUT_VAL){
                check_int(tmp[j]);
                //placeholder, write placement data to struct.
                op_id=SYM; 
                visitc++;
            }
            
            //What the fuck? Fix this.
            // Search for target identifier
            if (op_id == ID) {
                op_id += fl+1;

                switch (op_id) {
                    //Does the id exist? 
                    //Then proceed.
                    case SET_VAL:
                        id_index = check_valid(tmp[j], valid_id, valid_idc);
                        
                        //Proceed normally.
                    break;
                    
                    //check if identifier exists.
                    //move to sym if yes.
                    case PUT_VAL:
                        id_index = check_valid(tmp[j], valid_id_put, valid_putc);
                        op_id = SYM; 
                        //Proceed to SYM.
                        // Loop from SYM to PUT_VAL
                    break;
                    
                    // Move to sym. 
                    // From sym, to field check
                    case TILE:
                        op_id = SYM;
                    break;
                }
                if (id_index==valid_idc) { 
                    printf("Invalid identifier '%s'.\n", tmp[j]); 
                    exit(1); 
                }

            }

            //search for operation identifier
            if (op_id == OP_ID) {
                fl = check_valid(tmp[j], valid_ops, valid_opc); 
                if( fl == valid_opc) { 
                    printf("OP_ID: Invalid identifier '%s'.\nWith i = %d\n", tmp[j], i); 
                    printf("Previous was: %c\n", str[i-2]);
                    exit(1);
                }
                 
                op_id = ID; 
            }

            //token processed, reset for next token.
            j++;
            after_char = 0;
            first = SEARCH_RESET;
            last = SEARCH_RESET;
        }
        i++;
    }
}
