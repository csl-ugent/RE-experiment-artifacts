#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct vector vector_t;

vector_t* vector_new();
void vector_push_back(vector_t* v, void* elem);
size_t vector_size(vector_t* v);
void* vector_get(vector_t* v, size_t index);
#ifdef __cplusplus
}
#endif

#endif
