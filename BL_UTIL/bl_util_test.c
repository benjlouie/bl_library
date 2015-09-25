#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_util.h"

int int_cmp(const void *i1, const void *i2);

int main(void)
{
	int i = 0;
	size_t arr_size = 10;
	int *arr = NULL;
	size_t term = 0;
	void *tmp = NULL;
	clock_t start, end;
	void *pivot = NULL;
	
	srand(time(NULL));
	arr = malloc(sizeof(int) * arr_size);
	pivot = malloc(sizeof(int));
	

    for(i = 0; i < arr_size; i++) {
	    arr[i] = rand() % 10;// + rand() * 1000 + rand() * 1000000;
	}
	term = arr_size / 2;//rand() % arr_size;

	
	for(i = 0; i < arr_size; i++)
	    printf("%d, ", arr[i]);
	printf("\n");

	//*(int *)pivot = -1;//rand() % arr_size;
	//printf("pivot = %d\n", *(int *)pivot);
	start = clock();
	tmp = bl_util_qselect(arr, term, arr_size, sizeof(int), (int_cmp));
	//bl_util_partition(arr, pivot, arr_size, sizeof(int), (int_cmp));
	end = clock();
	if(tmp)
	   printf("term %u, qselect returned %d\n", (unsigned)term, *(int *)tmp);
	printf("time = %f\n", (double)(end - start) / CLOCKS_PER_SEC);
	
	
	for(i = 0; i < arr_size; i++)
	    printf("%d, ", arr[i]);
	printf("\n");
	
	
	return 0;
}

int int_cmp(const void *i1, const void *i2)
{
	return (*(int *)i1 > *(int *)i2) - (*(int *)i1 < *(int *)i2);
}
