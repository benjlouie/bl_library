/* BL_HEAP functions */
/*///////////////////*/

#include "bl_heap.h"

// TODO: make d-ary heap structure and functions

struct bl_heap_t {
    // heap data
    void **arr;
    size_t heapSize;
    size_t arraySize;
    int (*cmp_func)(const void *, const void *);
    
    // book keeping
    float growthRate;
    size_t constGrowthRate; // 0 if not a constant growth rate
};

// book keeping funcions
bl_heap *bl_heap_new(size_t initSize, int (*cmp_func)(const void *, const void *));
void bl_heap_set_growthRate(bl_heap *heap, float growthRate);
void bl_heap_set_constGrowthRate(bl_heap *heap, size_t constGrowthRate);
void bl_heap_trimToSize(bl_heap *heap, size_t size);
void bl_heap_free(bl_heap *heap);
// heap operations
void bl_heap_push(bl_heap *heap, void * const data);
void *bl_heap_peek(bl_heap *heap);
void *bl_heap_pop(bl_heap *heap);
void sift_up(bl_heap *heap, size_t index);
void sift_down(bl_heap *heap, size_t index);


// book keeping functions
void bl_heap_set_growthRate(bl_heap *heap, float growthRate)
{
    if(growthRate > 1) {
        heap->growthRate = growthRate;
    }
    heap->constGrowthRate = 0; // to ensure the new growth rate is used
}

void bl_heap_set_constGrowthRate(bl_heap *heap, size_t constGrowthRate)
{
    if(constGrowthRate != 0)
        heap->constGrowthRate = constGrowthRate;
}

void bl_heap_trimToSize(bl_heap *heap, size_t size)
{
    if(heap->heapSize > size) {
        size = heap->heapSize;
    }
    if(size == heap->arraySize) {
        return;
    }
    heap->arr = realloc(heap->arr, sizeof(void *) * size);
    heap->arraySize = size;
    printf("trimmed size = %lu\n", (long unsigned)heap->arraySize);
}

void bl_heap_free(bl_heap *heap)
{
    if(heap->arr)
        free(heap->arr);
    free(heap);
}

// heap operations
bl_heap *bl_heap_new(size_t initSize, int (*cmp_func)(const void *, const void *))
{
    bl_heap *heap = malloc(sizeof(bl_heap));
    heap->arr = malloc(sizeof(void *) * initSize);
    heap->heapSize = 0;
    heap->arraySize = initSize;
    heap->cmp_func = cmp_func;
    // book keeping variables
    heap->growthRate = 1.5;
    heap->constGrowthRate = 0;
    return heap;
}

void bl_heap_push(bl_heap *heap, void * const data)
{
    heap->heapSize++;
    if(heap->heapSize > heap->arraySize) { //needs more space
        if(heap->constGrowthRate != 0) {
            heap->arraySize = heap->arraySize + heap->constGrowthRate;
            heap->arr = realloc(heap->arr, sizeof(void *) * heap->arraySize);
        } else {
            heap->arraySize = heap->arraySize * heap->growthRate + 1;
            heap->arr = realloc(heap->arr, sizeof(void *) * heap->arraySize);
        }
        printf("arraysize = %lu\n", (long unsigned)heap->arraySize);
    }
    
    size_t index = heap->heapSize - 1;
    heap->arr[index] = data;
    sift_up(heap, index);
}

void *bl_heap_peek(bl_heap *heap)
{
    if(heap->heapSize > 0)
        return heap->arr[0];
    else
        return NULL;
}

void *bl_heap_pop(bl_heap *heap)
{
    void *retVal = NULL;
    
    if(heap->heapSize > 0) {
        retVal = heap->arr[0];
        heap->arr[0] = heap->arr[heap->heapSize - 1];
        heap->heapSize--;
        sift_down(heap, 0);
    }
    
    return retVal;
}

void sift_up(bl_heap *heap, size_t index)
{
    size_t parent;
    void *tmp;
    
    while(index != 0) {
        parent = (index - 1) / 2;
        if(heap->cmp_func(heap->arr[index], heap->arr[parent]) > 0) {
            tmp = heap->arr[parent];
            heap->arr[parent] = heap->arr[index];
            heap->arr[index] = tmp;
            index = parent;
        } else {
            break;
        }
    }
}

void sift_down(bl_heap *heap, size_t index)
{
    size_t lChild, rChild;
    size_t maxChild;
    void *tmp;
    
    while(index < heap->heapSize)
    {
        lChild = index * 2 + 1;
        rChild = index * 2 + 2;
        
        maxChild = lChild;
        if(lChild < heap->heapSize) {
            if(rChild < heap->heapSize) {
                // get max of l/rchild
                if(heap->cmp_func(heap->arr[rChild], heap->arr[lChild]) > 0) {
                    maxChild = rChild;
                }
            }
            
            // compare index to max child and swap
            if(heap->cmp_func(heap->arr[maxChild], heap->arr[index]) > 0) {
              tmp = heap->arr[index];
              heap->arr[index] = heap->arr[maxChild];
              heap->arr[maxChild] = tmp;
            } else {
                break;
            }
        }
        index = maxChild;
    }
}
