#include "../include/utils.h"

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

int check_valid(char* op, char** trgt, int limit){
    int i;
    for (i = 0; i<limit; i++){
        if (strcmp(op, trgt[i]) == 0){ break; }
    }

    return i;
}

void check_int(const char* num){
    int i;
    int fl;
    for(i = 0; i < strlen(num); i++){
        fl = isNum(num[i]);
        if (!fl) {printf("Expected int, got %c.\n", num[i]); exit(1);}
    }
}

void check_sym(const char *sym, int* op, int* op_prev, int* visits, int active_op){
    const char symbol = sym[0];
    char expected_c;
    
    printf("%d::%d::%c\n", *visits, active_op, symbol);
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
        printf("I sense... a tile.\n");
        exit(-1);
    }
}

