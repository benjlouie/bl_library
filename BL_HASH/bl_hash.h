/* Hash functions */
/*////////////////*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct bl_hashtable_t bl_hashtable;

bl_hashtable *bl_hashtable_new(size_t tableSize);
void bl_hashtable_put(bl_hashtable *ht, char *key, void *data);
void *bl_hashtable_get(bl_hashtable *ht, char *key);
void *bl_hashtable_remove(bl_hashtable *ht, char *key);
size_t bl_hashtable_count(bl_hashtable *ht);
void *bl_hashtable_modify(bl_hashtable *ht, char *key, void *newData);

// TODO: delete debug function
void printTable(bl_hashtable *ht);
