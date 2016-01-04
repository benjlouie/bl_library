#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_heap.h"

int int_cmp(const void *a, const void *b);

int main(void)
{
    int i = 0;
    int *arr;
    //int arr[10] = {5, 3, 0, 7, 7, 8, 8, 5, 5, 4};
    int arr_size = 16;
    
    arr = malloc(sizeof(int) * arr_size);
    
    srand(time(NULL));
    for(i = 0; i < arr_size; i++) {
        arr[i] = rand() % (arr_size * 10);
        printf("%d, ", arr[i]);
    }
    printf("\n");
    
    bl_heap *heap = bl_heap_new(100, (int_cmp));
    //bl_heap_set_growthRate(heap, 2.6);
    //bl_heap_set_constGrowthRate(heap, 10);
    for(i = 0; i < arr_size; i++) {
        bl_heap_push(heap, &arr[i]);
    }
    
    bl_heap_trimToSize(heap, 0);
    
    printf("%d\n", *(int *)bl_heap_peek(heap));
    
    for(i = 0; i < arr_size; i++) {
        int *tmp = bl_heap_pop(heap);
        printf("%d, ", *tmp);
    }
    
    bl_heap_free(heap);
    
    return 0;
}

int int_cmp(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) - (*(int *)a < *(int *)b);
}
