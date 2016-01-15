/* BL_HEAP functions */
/*///////////////////*/

#include "bl_heap.h"

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
// heap book keeping funcions
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

struct bl_dheap_t {
    // heap data
    void **arr;
    int dNum;
    size_t heapSize;
    size_t arraySize;
    int (*cmp_func)(const void *, const void *);
    
    // book keeping
    float growthRate;
    size_t constGrowthRate; // 0 if not a constant growth rate
};
// dheap book keeping funcions
bl_dheap *bl_dheap_new(size_t initSize, unsigned dNum, int (*cmp_func)(const void *, const void *));
void bl_dheap_set_growthRate(bl_dheap *dheap, float growthRate);
void bl_dheap_set_constGrowthRate(bl_dheap *dheap, size_t constGrowthRate);
void bl_dheap_trimToSize(bl_dheap *dheap, size_t size);
void bl_dheap_free(bl_dheap *dheap);
// dheap operations
void bl_dheap_push(bl_dheap *dheap, void * const data);
void *bl_dheap_peek(bl_dheap *dheap);
void *bl_dheap_pop(bl_dheap *dheap);
void dsift_up(bl_dheap *dheap, size_t index);
void dsift_down(bl_dheap *dheap, size_t index);


//// heap book keeping functions ////
/**
 * sets the multiplier rate the array grows at when it needs more space
 * @param heap the heap to modify
 * @param growthRate the new growth rate, must be greater than 1 for the rate to change
 */
void bl_heap_set_growthRate(bl_heap *heap, float growthRate)
{
    if(growthRate > 1) {
        heap->growthRate = growthRate;
        heap->constGrowthRate = 0; // to ensure the new growth rate is used
    }
}

/**
 * sets the growth rate to a constant value when more space is needed
 * @param heap the heap to modify
 * @param constGrowthRate the constant number of array spaces to grow by
 */
void bl_heap_set_constGrowthRate(bl_heap *heap, size_t constGrowthRate)
{
    if(constGrowthRate != 0)
        heap->constGrowthRate = constGrowthRate;
}

/**
 * Resizes to the heap array to the specified size or to the minimum space needed
 * @param heap the heap to modify
 * @param size new size of the array, if less than minimum needed: the minimum size will be used
 * @note call with size=0 to shrink the heap to the minimum size needed
 */
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
}

/**
 * frees the heap
 * @param heap the heap to free
 */
void bl_heap_free(bl_heap *heap)
{
    if(heap->arr)
        free(heap->arr);
    free(heap);
}

//// heap operations ////
/**
 * creates a new heap
 * @param initSize initial size of the array containing the heap
 * @param cmp_func() the function used to compare elements of the heap
 * @note the cmp_func() determines if it is a min or max heap
 * @return the newly created heap
 */
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

/**
 * add an element to the heap
 * @param heap the heap to add to
 * @param data pointer to the data that will be added
 * @note data will be compared using cmp_func()
 * @see bl_heap_new()
 */
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
    }
    
    size_t index = heap->heapSize - 1;
    heap->arr[index] = data;
    sift_up(heap, index);
}

/**
 * peek at the max/min element of the heap
 * @param heap the heap
 * @return pointer to the data of the min/max element
 */
void *bl_heap_peek(bl_heap *heap)
{
    if(heap->heapSize > 0)
        return heap->arr[0];
    else
        return NULL;
}

/**
 * removes the min/max element of the heap
 * @param heap the heap to pop from
 * @return pointer to the data of the popped element
 */
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

/**
 * ensures the index element is correct going towards the root, sifts up in a way
 * @param heap the heap
 * @param index index of the element to sift up
 */
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

/**
 * ensures the index element is correct going towards the leaves, sifts down in a way
 * @param heap the heap
 * @param index index of the element to be sifted down
 */
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



//// dheap book keeping funcions ////

/**
 * sets the multiplier rate the array grows at when it needs more space
 * @param dheap the d-ary heap to modify
 * @param growthRate the new growth rate, must be greater than 1 for the rate to change
 */
void bl_dheap_set_growthRate(bl_dheap *dheap, float growthRate)
{
    if(growthRate > 1) {
        dheap->growthRate = growthRate;
        dheap->constGrowthRate = 0; // to ensure the new growth rate is used
    }
}

/**
 * sets the growth rate to a constant value when more space is needed
 * @param dheap the d-ary heap to modify
 * @param constGrowthRate the constant number of array spaces to grow by
 */
void bl_dheap_set_constGrowthRate(bl_dheap *dheap, size_t constGrowthRate)
{
    if(constGrowthRate != 0)
        dheap->constGrowthRate = constGrowthRate;
}

/**
 * Resizes to the heap array to the specified size or to the minimum space needed
 * @param dheap the d-ary heap to modify
 * @param size new size of the array, if less than minimum needed: the minimum size will be used
 * @note call with size=0 to shrink the d-ary heap to the minimum size needed
 */
void bl_dheap_trimToSize(bl_dheap *dheap, size_t size)
{
    if(dheap->heapSize > size) {
        size = dheap->heapSize;
    }
    if(size == dheap->arraySize) {
        return;
    }
    dheap->arr = realloc(dheap->arr, sizeof(void *) * size);
    dheap->arraySize = size;
}

/**
 * frees the d-ary heap
 * @param dheap the d-ary heap to free
 */
void bl_dheap_free(bl_dheap *dheap)
{
    if(dheap->arr)
        free(dheap->arr);
    free(dheap);
}

//// dheap operations ////

/**
 * creates a new d-ary heap
 * @param initSize initial size of the array containing the heap
 # @param dNum number of children per element (Ex: 2-binary, 3-ternary, etc...)
 * @param cmp_func() the function used to compare elements of the heap
 * @note the cmp_func() determines if it is a min or max heap
 * @return the newly created d-ary heap
 */
bl_dheap *bl_dheap_new(size_t initSize, unsigned dNum, int (*cmp_func)(const void *, const void *))
{
    if(dNum == 0) {
        errno = EINVAL;
        return NULL;
    }
    bl_dheap *dheap = malloc(sizeof(bl_dheap));
    dheap->arr = malloc(sizeof(void *) * initSize);
    dheap->dNum = dNum;
    dheap->heapSize = 0;
    dheap->arraySize = initSize;
    dheap->cmp_func = cmp_func;
    // book keeping variables
    dheap->growthRate = 1.5;
    dheap->constGrowthRate = 0;
    return dheap;
}

/**
 * add an element to the d-ary heap
 * @param dheap the d-ary heap to add to
 * @param data pointer to the data that will be added
 * @note data will be compared using cmp_func()
 * @see bl_dheap_new()
 */
void bl_dheap_push(bl_dheap *dheap, void * const data)
{
    dheap->heapSize++;
    if(dheap->heapSize > dheap->arraySize) { //needs more space
        if(dheap->constGrowthRate != 0) {
            dheap->arraySize = dheap->arraySize + dheap->constGrowthRate;
            dheap->arr = realloc(dheap->arr, sizeof(void *) * dheap->arraySize);
        } else {
            dheap->arraySize = dheap->arraySize * dheap->growthRate + 1;
            dheap->arr = realloc(dheap->arr, sizeof(void *) * dheap->arraySize);
        }
    }
    
    size_t index = dheap->heapSize - 1;
    dheap->arr[index] = data;
    dsift_up(dheap, index);
}

/**
 * peek at the max/min element of the d-ary heap
 * @param dheap the d-ary heap
 * @return pointer to the data of the min/max element
 */
void *bl_dheap_peek(bl_dheap *dheap)
{
    if(dheap->heapSize > 0)
        return dheap->arr[0];
    else
        return NULL;
}

/**
 * removes the min/max element of the d-ary heap
 * @param dheap the d-ary heap to pop from
 * @return pointer to the data of the popped element
 */
void *bl_dheap_pop(bl_dheap *dheap)
{
    void *retVal = NULL;
    
    if(dheap->heapSize > 0) {
        retVal = dheap->arr[0];
        dheap->arr[0] = dheap->arr[dheap->heapSize - 1];
        dheap->heapSize--;
        dsift_down(dheap, 0);
    }
    
    return retVal;
}

/**
 * ensures the index element is correct going towards the root, sifts up in a way
 * @param dheap the d-ary heap
 * @param index index of the element to sift up
 */
void dsift_up(bl_dheap *dheap, size_t index)
{
    size_t parent;
    void *tmp;
    
    while(index != 0) {
        parent = (index - 1) / dheap->dNum;
        if(dheap->cmp_func(dheap->arr[index], dheap->arr[parent]) > 0) {
            tmp = dheap->arr[parent];
            dheap->arr[parent] = dheap->arr[index];
            dheap->arr[index] = tmp;
            index = parent;
        } else {
            break;
        }
    }
}

/**
 * ensures the index element is correct going towards the leaves, sifts down in a way
 * @param dheap the d-ary heap
 * @param index index of the element to be sifted down
 */
void dsift_down(bl_dheap *dheap, size_t index)
{
    size_t curChild;
    size_t maxChild, maxIndex;
    void *tmp;
    
    while(index < dheap->heapSize)
    {
        maxChild = index * dheap->dNum + 1;
        if(maxChild > dheap->heapSize)
            break;
        maxIndex = index * dheap->dNum + dheap->dNum + 1; // +1 to work with next for-loop
        if(maxIndex > dheap->heapSize)
            maxIndex = dheap->heapSize;
        // find max child
        for(curChild = maxChild + 1; curChild < maxIndex; curChild++) {
            if(dheap->cmp_func(dheap->arr[curChild], dheap->arr[maxChild]) > 0) {
                maxChild = curChild;
            }
        }
        // compare max child to index, swap
        if(dheap->cmp_func(dheap->arr[maxChild], dheap->arr[index]) > 0) {
            tmp = dheap->arr[index];
            dheap->arr[index] = dheap->arr[maxChild];
            dheap->arr[maxChild] = tmp;
            index = maxChild;
        } else {
            break;
        }
    }
}
