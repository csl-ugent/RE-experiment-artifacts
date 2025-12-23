#include "vector.h"

#include <cstddef>
#include <vector>

typedef struct vector {
  std::vector<void*> inner;
} vector_t;

vector_t* vector_new()
{
  return new vector();
}

void vector_push_back(vector_t* v, void* elem)
{
  v->inner.push_back(elem);
}

size_t vector_size(vector_t* v)
{
  return v->inner.size();
}

void* vector_get(vector_t* v, size_t index)
{
  return v->inner[index];
}
