#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

enum op_status {OP_ID = 0, ID, SET_VAL, PUT_VAL, SYM};
enum id_set {WIDTH = 0, HEIGHT, TILE_SIZE};

#define validc 2
const char* valid_ops[] = {
    "set",
    "put"
};  

#define valid_idc 2
const char* valid_id[] = {
    "X",
    "Y"
};

// const char* valid_fields[] = {
//
// };

// width height info
int dim_vars[2] = {0, 0};

//store new entity identifiers here
char** entities;

#define isNum(arg)(arg>='0' && arg<='9')
#define isSym(arg)(strcmp(arg, ";"))

void parse(char* str);

int main(int argc, char** argv){
    if (argc != 2){
        printf("Error: Invalid number of arguments.\nUsage: mapper in.txt\n");
        exit(1);
    }
    
    char* fstring = ftostr(argv[argc-1]); //copy full file to string
    parse(fstring);
    
    free(fstring);
    return 0;
}


void parse(char* str){
    int op_id = OP_ID;

    int first = SEARCH_RESET;
    int last = SEARCH_RESET;
    
    int id_index;
    int j = 0;
    char tmp[3][32];

    int fl = -1;
    int after_char = 0;
    int white_or_new = 0;
    int symbol = 0;
    
    int i = 0;
    char c = str[i];
    while (c!='\0') {
        c = str[i];
        
        white_or_new = (c == ' ') || (c == '\n');
        symbol = ( c == ';' );

        if (!white_or_new && first == SEARCH_RESET) { after_char = 1; first = i;}
        if (white_or_new && !after_char) { i++; c = str[i]; continue; } 
        if (after_char && (white_or_new || symbol)) {last = i; i--;}
        
        if (first != SEARCH_RESET && last != SEARCH_RESET){
            //copy to temp char* and null terminate.
            int n_offset = (last-first) ? last-first : (last-first) + 1;
            memcpy(tmp[j], str+first, (last-first)+1);
            tmp[j][n_offset] = '\0';
            
            printf("Token number: %d as %s\n", j, tmp[j]);
            
            //SYM, or ; is the last search state. Process, and reset
            if (op_id == SYM) {
                op_id = (isSym(tmp[j]) == 0) 
                    ? OP_ID 
                    : printf("Expecting ';' at end of statement - got %s.\n", tmp[j]); 
                
                op_id = OP_ID;
                j = 0;
                i+=2; //currently i points to last digit before ;. advance twice.
                after_char = 0;
                first = SEARCH_RESET;
                last = SEARCH_RESET;
                printf("Block finished with i=%d.\n\n", i);
                continue;
            }

            // Search for the set value. Expect int.
            if (op_id == SET_VAL){
                
                check_int(tmp[j]);
                dim_vars[id_index] = atoi(tmp[j]); 

                printf("Identifier %s SET to '%d'\n\n", tmp[1], atoi(tmp[j]));
                op_id=SYM;

            }
            
            //searching for put value.
            if (op_id == PUT_VAL){
                op_id=SYM; 
            }
            
            // Search for target identifier
            if (op_id == ID) {
                id_index = (fl==0) 
                    ? check_valid(tmp[j], valid_id, valid_idc)
                    : printf("Sorry, put action not supported yet!\n");
                if (id_index==valid_idc) { printf("Invalid identifier '%s'.\n", tmp[j]); exit(1); }

                printf("Working with %s...\n", tmp[j]);
                op_id = (fl==0) ? SET_VAL : PUT_VAL;
            }

            //search for operation identifier
            if (op_id == OP_ID) {
                fl = check_valid(tmp[j], valid_ops, validc); 
                if( fl == validc) { printf("Invalid identifier '%s'.\n", tmp[j]); exit(1);}
                
                printf("Working with %s...\n", tmp[j]);
                op_id = ID;
            }

            //token processed, reset for next token.
            //Also advance to next read state
            j++;
            after_char = 0;
            first = SEARCH_RESET;
            last = SEARCH_RESET;
        }
        i++;
    }
}
