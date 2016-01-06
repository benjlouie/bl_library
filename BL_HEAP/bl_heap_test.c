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
    int arr_size = 17;
    
    arr = malloc(sizeof(int) * arr_size);
    
    srand(time(NULL));
    for(i = 0; i < arr_size; i++) {
        arr[i] = rand() % (arr_size * 10);
        printf("%d, ", arr[i]);
    }
    printf("\n");
    
    bl_dheap *dheap = bl_dheap_new(10, 4, (int_cmp));
    //bl_dheap_set_growthRate(dheap, 2.6);
    //bl_dheap_set_constGrowthRate(dheap, 11);
    for(i = 0; i < arr_size; i++) {
        bl_dheap_push(dheap, &arr[i]);
    }
    
    bl_dheap_trimToSize(dheap, 0);
    
    printf("%d\n", *(int *)bl_dheap_peek(dheap));
    
    for(i = 0; i < arr_size; i++) {
        int *tmp = bl_dheap_pop(dheap);
        printf("%d, ", *tmp);
    }
    
    bl_dheap_free(dheap);
    
    return 0;
}

int int_cmp(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) - (*(int *)a < *(int *)b);
}
