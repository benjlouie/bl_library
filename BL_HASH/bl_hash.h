/* Hash functions */
/*////////////////*/
#pragma once
#ifndef BL_HASH_H
#define BL_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct bl_hashtable_t bl_hashtable;

bl_hashtable *bl_hashtable_new(size_t tableSize);
void bl_hashtable_insert(bl_hashtable *ht, void *key, size_t keyLength, void *data);
void *bl_hashtable_get(bl_hashtable *ht, void *key, size_t keyLength);
void *bl_hashtable_remove(bl_hashtable *ht, void *key, size_t keyLength);
size_t bl_hashtable_count(bl_hashtable *ht);
size_t bl_hashtable_tablesize(bl_hashtable *ht);
float bl_hashtable_loadfactor(bl_hashtable *ht);
void *bl_hashtable_modify(bl_hashtable *ht, void *key, size_t keyLength, void *newData);
void bl_hashtable_foreach(bl_hashtable *ht, void *userData, void (*func)(char *key, void *data, void *userData));
void bl_hashtable_foreach_remove(bl_hashtable *ht, void *userData, void (*func)(char *key, void *data, void *userData));
void bl_hashtable_rehash(bl_hashtable *ht, size_t newTableSize);
void bl_hashtable_rehash_auto(bl_hashtable *ht, float minFactor, float maxFactor);
void bl_hashtable_free(bl_hashtable *ht);

// TODO: delete debug function
void printTable(bl_hashtable *ht);

#endif

