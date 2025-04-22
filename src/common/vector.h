#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct {
    size_t back;
    size_t front;

    size_t elements;
    void** data_array;
} vector_t;

void shift_right(vector_t* v, const size_t index);
void shift_left(vector_t* v, const size_t index);
void** double_vector(vector_t* v);

vector_t* init_vector();
void free_vector(vector_t* v);

void* at(const vector_t* v, const size_t index);
size_t get_index(const vector_t* v);

void overwrite_at(const vector_t* v, const void* data, const size_t size, const size_t index);
void insert(vector_t* v, const size_t index, const void* data, const size_t size);
void push_back(vector_t* v, const void* data, const size_t size);

void pop_at(vector_t* v, const size_t index);
void pop_back(vector_t* v);

#endif

#ifdef VECTOR_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

inline void** double_vector(vector_t* v) {
    v->elements *= 2;
    void** temp = realloc(v->data_array, v->elements * sizeof(void*));
    if (temp == NULL) {
        fprintf(stderr, "Coud not expand vector!\n");
    }
    
    return temp;
}
inline void shift_right(vector_t* v, const size_t index) {
    if (v->back+1 > v->elements) {
        v->data_array = double_vector(v);
    }

    size_t head = v->back;
    while (head > index) {
        v->data_array[head] = v->data_array[head-1];
        head--;
    }

    v->back++;
}
inline void shift_left(vector_t* v, const size_t index) {
    size_t i = index;
    while (i+1 < v->back) {
        v->data_array[i] = v->data_array[i+1];
        i++;
    }

    v->back--;
}

inline vector_t* init_vector() {
    vector_t* v = malloc(sizeof(vector_t));

    v->back = 0;
    v->front = 0;
    v->elements = 8;
    v->data_array = NULL;

    v->data_array = malloc(sizeof(void*) * v->elements);
    if (v->data_array == NULL) {
        fprintf(stderr, "Reallocation failed\n");
        exit(EXIT_FAILURE);
    }

    return v;
}
inline void free_vector(vector_t* v) {

    for (size_t i = 0; i < v->back; i++) {
        free(v->data_array[i]);
    }

    free(v->data_array);
    free(v);
}

inline void* at(const vector_t* v, const size_t index) {
    if (index > v->back) {
        fprintf(stderr, "at: Index out of range.\n");
        exit(EXIT_FAILURE);
    }
    return v->data_array[index];
}
inline size_t get_index(const vector_t* v) {
    return v->back-1;
}

inline void overwrite_at(const vector_t* v, const void* data, const size_t size, const size_t index) {
    if (index > v->back) {
        fprintf(stderr, "overwrite_at: Index out of range.\n");
        exit(EXIT_FAILURE);
    }

    free(v->data_array[index]);
    v->data_array[index] = NULL;

    v->data_array[index] = malloc(size);
    if (v->data_array[v->back] == NULL)
        exit(EXIT_FAILURE);

    memcpy(v->data_array[v->back], data, size);
}
inline void insert(vector_t* v, const size_t index, const void* data, const size_t size) {
    if (index > v->back) {
        fprintf(stderr, "Insert: Index out of range.\n");
        exit(EXIT_FAILURE);
    }

    shift_right(v, index);

    v->data_array[index] = malloc(size);
    if (v->data_array[index] == NULL)
        exit(EXIT_FAILURE);

    memcpy(v->data_array[index], data, size);
}
inline void push_back(vector_t* v, const void* data, const size_t size) {
    v->data_array[v->back] = malloc(size);
    if (v->data_array[v->back] == NULL)
        exit(EXIT_FAILURE);

    memcpy(v->data_array[v->back], data, size);

    v->back++;
    if (v->back >= v->elements) {
        v->data_array = double_vector(v);
    }
}

inline void pop_at(vector_t* v, const size_t index) {
    if (index > v->back) {
        fprintf(stderr, "Pop: Index out of range.\n");
        exit(EXIT_FAILURE);
    }

    free(v->data_array[index]);
    v->data_array[index] = NULL;

    // shift elements left
    shift_left(v, index);
}
inline void pop_back(vector_t* v) {
    free(v->data_array[v->back-1]);
    v->data_array[v->back-1] = NULL;

    v->back--;
}

#endif
