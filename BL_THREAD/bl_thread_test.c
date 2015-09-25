#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_thread.h"

int int_cmp(const void *a, const void *b);


int main(void)
{
    size_t i = 0;
    clock_t start, end;
    size_t arr_size = 1000000;
    int *arr = malloc(sizeof(int) * arr_size);
    //int arr[10] = {2, 8, 9, 5, 0, 8, 5, 3, 8, 5};
    
    srand(time(NULL));
    for(i = 0; i < arr_size; i++) {
        arr[i] = rand() % 100000 + rand() * 100000;
    }
    
    /*for(i = 0; i < arr_size; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");*/

    start = clock();
	//bl_thread_print_test(10);
	//bl_thread_join_test(10);
	//bl_thread_mutex_test(10);
	//bl_thread_stack_test(10);
	//bl_thread_cond_test(10); /* FIX */
	
    bl_thread_qsort(arr, arr_size, 4);
	//qsort(arr, arr_size, sizeof(int), (int_cmp));
	end = clock();
	printf("time = %f\n", (float)(end - start) / CLOCKS_PER_SEC);
	
	/*for(i = 0; i < arr_size; i++) {
        printf("%d, ", arr[i]);
    }
    printf("\n");*/
	
	int test = arr[0];
    i = 1;
    while(i < arr_size) {
        if(test > arr[i]) {
            printf("NOT SORTED\n");
            break;
        }
        test = arr[i];
        i++;
    }
	
	pthread_exit(NULL);
	return 0;
}

int int_cmp(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) - (*(int *)a < *(int *)b);
}
