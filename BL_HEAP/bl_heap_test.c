#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_heap.h"

int int_cmp(const void *a, const void *b);
void add(void *data, void *userData);
void randNum(void *data, void *userData);

int main(void)
{
    int i = 0;
    int *arr;
    //int arr[17] = {46, 84, 160, 27, 73, 152, 77, 77, 14, 122, 118, 37, 153, 1, 65, 114, 160};
    int arr_size = 17;
    
    arr = malloc(sizeof(int) * arr_size);
    
    srand(time(NULL));
    while(1) {
        for(i = 0; i < arr_size; i++) {
            arr[i] = rand() % (arr_size * 10);
            printf("%d, ", arr[i]);
        }
        printf("\n");

        //bl_dheap *dheap = bl_dheap_new(10, 4, (int_cmp));
        //bl_dheap_set_growthRate(dheap, 2.6);
        //bl_dheap_set_constGrowthRate(dheap, 11);
        bl_heap *heap = bl_heap_new(10, int_cmp);
        for(i = 0; i < arr_size; i++) {
            //bl_dheap_push(dheap, &arr[i]);
            bl_heap_push(heap, &arr[i]);
        }

        //bl_dheap_trimToSize(dheap, 0);
        bl_heap_trimToSize(heap, 0);

        //printf("%d\n", *(int *)bl_dheap_peek(dheap));
        printf("%d\n", *(int *)bl_heap_peek(heap));

        int addNum = 6;
        //bl_dheap_foreach(dheap, &addNum, (add));
        bl_heap_foreach(heap, &addNum, (add));
        //int randN = 100;
        //bl_dheap_foreach(dheap, &randN, (randNum));
        //bl_dheap_heapify(dheap);
        //bl_dheap_foreach_remove(dheap, NULL, NULL);

        for(i = 0; i < arr_size; i++) {
            //int *tmp = bl_dheap_pop(dheap);
            int *tmp = bl_heap_pop(heap);
            if(tmp) {
                    printf("%d, ", *tmp);
            } else {
                break;
            }
        }
        printf("\n\n");

        //bl_dheap_free(dheap);
        bl_heap_free(heap);
    }
    
    return 0;
}

int int_cmp(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) - (*(int *)a < *(int *)b);
}

void add(void *data, void *userData)
{
    *(int *)data += *(int *)userData;
}

void randNum(void *data, void *userData)
{
    *(int *)data = rand() % *(int *)userData;
}
