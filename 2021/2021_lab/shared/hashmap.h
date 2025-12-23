#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct hashmap hashmap_t;

hashmap_t *hashmap_new(size_t cap);
void hashmap_free(hashmap_t *map);
void *hashmap_get(const hashmap_t *map, unsigned key);
void *hashmap_set(hashmap_t *map, unsigned key, void *item);

#endif
