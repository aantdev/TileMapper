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
    printf("%s\n", fstring); 
    parse(fstring);
    
    free(fstring);
    return 0;
}


void parse(char* str){
    int i = 0;
    char c = str[i];
    
    int op_id = OP_ID;

    int first = SEARCH_RESET;
    int last = SEARCH_RESET;
    
    int j = 0;
    char tmp[3][32];

    int fl = -1;
    int after_char = 0;
    int white_or_new = 0;
    int symbol = 0;
    while (c!='\0') {
        //Step one: Look for id1
        white_or_new = (c == ' ') || (c == '\n');
        symbol = ( c == ';' );
        
        printf("%d,%c\n", i, c);

        if (!white_or_new && first == SEARCH_RESET) { after_char = 1; first = i;}
        if (white_or_new && !after_char) { i++; c = str[i]; continue; } 
        if (after_char && (white_or_new || symbol)) { last = i; }
        
        if (first != SEARCH_RESET && last != SEARCH_RESET){
            int n_offset = (last-first) ? last-first : (last-first) + 1;
            memcpy(tmp[j], str+first, (last-first)+1);
            tmp[j][n_offset] = '\0';
            
            printf("%d-%s\n", j, tmp[j]);
    
            if (op_id == SYM) {
                op_id = (isSym(tmp[j]) == 0) 
                    ? OP_ID 
                    : printf("Expecting ';' at end of statement - got %s.\n", tmp[j]); exit(1); 
                
                op_id = OP_ID;
                j = 0;
                i++; c = str[i];
                after_char = 0;
                first = SEARCH_RESET;
                last = SEARCH_RESET;
                continue;
            }

            //are we looking for the operation identifier?
            if (op_id == SET_VAL){
                
                check_int(tmp[j]);

                printf("Alright, I've SET the identifier to :'%s', %d\n\n", tmp[j], symbol);
                op_id= (symbol) ? OP_ID : SYM;
                if (op_id == OP_ID) {
                    j = 0;
                    i++; c = str[i];
                    after_char = 0;
                    first = SEARCH_RESET;
                    last = SEARCH_RESET;
                    continue;
                }

            }

            if (op_id == PUT_VAL){
                op_id=SYM; 
            }
            
            // is this the id?
            if (op_id == ID) {
                int fl_ = (fl==0) 
                    ? check_valid(tmp[j], valid_id, valid_idc)
                    : printf("Sorry, put action not supported yet!\n");
                if (fl_==valid_idc) { printf("Invalid identifier '%s'.\n", tmp[j]); exit(1); }

                printf("Working with %s...\n", tmp[j]);
                op_id = (fl==0) ? SET_VAL : PUT_VAL;
            }

            
            if (op_id == OP_ID) {
                fl = check_valid(tmp[j], valid_ops, validc); 
                if( fl == validc) { printf("Invalid identifier '%s'.\n", tmp[j]); exit(1);}
                
                printf("Working with %s...\n", tmp[j]);
                op_id = ID;
            }


            j++;
            after_char = 0;
            first = SEARCH_RESET;
            last = SEARCH_RESET;
        }
        i++;
        c = str[i];
    }
}
