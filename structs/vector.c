#include <stdlib.h>
#include "vector.h"
#include "pair.h"

#define DEFINE_VECTOR(T)    \
void Vector_##T##_init(Vector_##T *vec) {   \
    vec -> capacity = 4;    \
    vec -> size = 0;    \
    vec -> vals = malloc(vec -> capacity * sizeof(T));  \
    if(!vec -> vals) { \
        printf("couldn't allocate memory\n");   \
        exit(EXIT_FAILURE); \
    }   \
}   \
\
void Vector_##T##_deinit(Vector_##T *vec) { \
    if(!vec) return;    \
    vec -> capacity = vec -> size = 0;  \
    free(vec -> vals);  \
}   \
\
Vector_##T* Vector_##T##_new() {    \
    Vector_##T* vec = malloc(sizeof(Vector_##T));   \
    if(!vec) {  \
        printf("couldn't allocate memory\n");   \
        exit(EXIT_FAILURE); \
    }   \
    Vector_##T##_init(vec); \
    return vec; \
}   \
\
void Vector_##T##_free(Vector_##T *vec) {   \
    if(!vec) return;    \
    vec -> capacity = vec -> size = 0;  \
    free(vec -> vals);  \
    free(vec);  \
}   \
\
void Vector_##T##_push_back(Vector_##T *vec, T val) {   \
    if(vec -> size == vec -> capacity) {    \
        vec -> capacity *= 2;   \
        T *tmp = realloc(vec -> vals, vec -> capacity * sizeof(T));    \
        if(!tmp) {  \
            printf("couldn't allocate memory\n");   \
            exit(EXIT_FAILURE); \
        } \
        vec -> vals = tmp;  \
    }   \
    vec -> vals[vec -> size++] = val;  \
}   \
\
T Vector_##T##_pop_back(Vector_##T *vec) {  \
    if(vec -> size == 0) { \
            printf("vector size == 0, you can't pop back\n");   \
            exit(EXIT_FAILURE); \
    }   \
    return vec -> vals[--vec -> size]; \
}   \
\
void Vector_##T##_clear(Vector_##T *vec) {  \
    vec -> size = 0;    \
}   \
\
void Vector_##T##_reverse(Vector_##T *vec) {    \
    for(int i = 0; i < vec -> size / 2; i++) {  \
        T temp = vec -> vals[i];    \
        vec -> vals[i] = vec -> vals[vec -> size - i - 1];  \
        vec -> vals[vec -> size - i - 1] = temp;    \
    }   \
}   \

DEFINE_VECTOR(Pair_int)
DEFINE_VECTOR(int)
