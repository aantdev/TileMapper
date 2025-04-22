#include "file_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* file_read(char *path){
    //try to open file
    FILE* fptr;
    if ((fptr = fopen(path, "rb")) == NULL){
        perror("Error: ");
        return NULL;
    }
    char* final = NULL;

    // allocate space, write contents to space
    if (fseek(fptr, 0, SEEK_END) != 0) {
        exit(EXIT_FAILURE);
    }
    long len = ftell(fptr);
    if (len == -1) {
        exit(EXIT_FAILURE);
    }
    rewind(fptr);
    final = malloc(sizeof(char) * len+1);

    size_t shit_read = fread(final, sizeof(char), len, fptr);
    if (shit_read != (size_t)len) {
        printf("Shit hit the fan\n");
        exit(EXIT_FAILURE);
    }

    final[len] = '\0';

    fclose(fptr);
    return final;
}

