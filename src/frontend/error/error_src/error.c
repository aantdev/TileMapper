#include "error.h"
#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static struct error_manager{
    bool fatal_state;
    vector_t* error_v;
    
} error_m = {.fatal_state = false};

void error_if(void* ptr) {
	if(ptr == NULL) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}
}

void error_init(void) {
    error_m.error_v = init_vector();
    error_if(error_m.error_v);
}

void error_submit(error_t error, bool fatal) {
    push_back(error_m.error_v, &error, sizeof(error));
    if (fatal)
        error_final();
}

void error_final(void) {
    if (error_m.error_v->back == 0) {
        printf("\nFinished successfully, bye-bye!\n");
        exit(EXIT_SUCCESS);
    }
    
    vector_t* error_v = error_m.error_v;
    for (size_t i = 0; i < error_v->back; i++) {
        error_t* packet = at(error_v, i);
        printf("In file:%s, %lu::%lu ERROR: %s\n", 
               packet->file_path, packet->line, packet->col, packet->message);
    }

    free_vector(error_v);
    exit(EXIT_FAILURE);
}
