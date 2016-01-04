/* Heap functions */
/*////////////////*/
#ifndef BL_HEAP_H
#define BL_HEAP_H

#include <stdio.h>
#include <stdlib.h>

typedef struct bl_heap_t bl_heap;

// book keeping functions
bl_heap *bl_heap_new(size_t initSize, int (*cmp_func)(const void *, const void *));
void bl_heap_set_growthRate(bl_heap *heap, float growthRate);
void bl_heap_set_constGrowthRate(bl_heap *heap, size_t constGrowthRate);
void bl_heap_trimToSize(bl_heap *heap, size_t size);
void bl_heap_free(bl_heap *heap);
// heap operations
void bl_heap_push(bl_heap *heap, void * const data);
void *bl_heap_peek(bl_heap *heap);
void *bl_heap_pop(bl_heap *heap);

#endif
