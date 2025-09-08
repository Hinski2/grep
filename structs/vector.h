#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include "pair.h"

#define DEFINE_VECTOR_PROTOTYPE(T) \
typedef struct { \
    size_t size, capacity; \
    T* vals; \
} Vector_##T; \
\
void Vector_##T##_init(Vector_##T *vec); \
void Vector_##T##_deinit(Vector_##T *vec); \
Vector_##T* Vector_##T##_new(); \
void Vector_##T##_free(Vector_##T *vec); \
\
void Vector_##T##_push_back(Vector_##T *vec, T val); \
T Vector_##T##_pop_back(Vector_##T *vec); \
void Vector_##T##_clear(Vector_##T *vec); \


DEFINE_VECTOR_PROTOTYPE(Pair_int)

#endif