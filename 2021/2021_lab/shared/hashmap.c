#include "hashmap.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bucket {
    unsigned key;
    void* item;
} bucket_t;

typedef struct hashmap {
    size_t cap;
    bucket_t* buckets;
} hashmap;

static bucket_t *bucket_at(const hashmap_t *map, size_t index) {
    return &map->buckets[index];
}

static size_t get_hash(const hashmap_t *map, size_t key) {
    return key % map->cap;
}

static size_t next_index(const hashmap_t *map, size_t index) {
    return (index +1) % map->cap;
}

// hashmap_new returns a new hash map. 
// Param `cap` is the capacity of the hashmap
hashmap_t *hashmap_new(size_t cap)
{
    hashmap_t* ret = malloc(sizeof(hashmap_t));
    ret->cap = cap;
    ret->buckets = calloc(cap, sizeof(bucket_t));

    return ret;
}

// hashmap_free frees the hash map
void hashmap_free(hashmap_t *map) {
    if (!map) return;
    free(map->buckets);
    free(map);
}

// hashmap_set inserts an item in the hash map. If an item is
// inserted then it is returned otherwise NULL is returned.
void *hashmap_set(hashmap_t *map, unsigned key, void *item) {
    if (!key)
        return NULL;

    // Keep looking for an empty bucket.. OR a bucket with the same key!
    // We're assuming here the map never gets full. Hooray!
    size_t index = get_hash(map, key);
    bucket_t* bucket = bucket_at(map, index);
    while(bucket->key && bucket->key != key) {
        index  = next_index(map, index);
        bucket = bucket_at(map, index);
    }

    // Fill in bucket and return
    bucket->key = key;
    bucket->item = item;
    return bucket->item;
}

// hashmap_get returns the item based on the provided key. If the item is not
// found then NULL is returned.
void *hashmap_get(const hashmap_t *map, unsigned key) {
    if (!key)
        return NULL;

    // Keep looking for bucket that matches key
    // If we come upon an empty bucket, we failed
    // We're assuming here the map never gets full. Hooray!
    size_t index = get_hash(map, key);
    bucket_t* bucket = bucket_at(map, index);
    while(bucket->key != key && bucket->key) {
        index  = next_index(map, index);
        bucket = bucket_at(map, index);
    }

    if (bucket->key)
        return bucket->item;
    else
        return NULL;
}
