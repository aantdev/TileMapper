#include "file_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* file_read(char *path){
    //try to open file
    FILE* fptr;
    if ((fptr = fopen(path, "rb")) == NULL)
        return NULL;
    
    char* final = NULL;

    // allocate space, write contents to space
    if (fseek(fptr, 0, SEEK_END) != 0)
        return NULL;

    long len = ftell(fptr);
    if (len == -1)
        return NULL;

    rewind(fptr);
    final = malloc(sizeof(char) * len+1);

    size_t data_read = fread(final, sizeof(char), len, fptr);
    if (data_read != (size_t)len) 
        return NULL;

    final[len] = '\0';
    fclose(fptr);
    return final;
}

