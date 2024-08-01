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

int check_valid(const char* op, const char** trgt, int limit){
    int i;
    for (i = 0; i<2; i++){
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
