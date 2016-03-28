#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bl_util.h"

int int_cmp(const void *i1, const void *i2);

int main(void)
{
	int i = 0;
	size_t arr_size = 14;
	int *arr = NULL;
	size_t term = 0;
	void *tmp = NULL;
	int tmpNum = 0;
	clock_t start, end;
	void *pivot = NULL;
	
	srand(time(NULL));
	arr = malloc(sizeof(int) * arr_size);
	pivot = malloc(sizeof(int));
	

    for(i = 0; i < arr_size; i++) {
	    //arr[i] = rand() % 10;// + rand() * 1000 + rand() * 1000000;
	    arr[i] = i;
	}
	term = arr_size / 2;//rand() % arr_size;

	
	for(i = 0; i < arr_size; i++)
	    printf("%d, ", arr[i]);
	printf("\n");

	//*(int *)pivot = -1;//rand() % arr_size;
	//printf("pivot = %d\n", *(int *)pivot);
	/*start = clock();
	tmp = bl_util_qselect(arr, term, arr_size, sizeof(int), (int_cmp));
	//bl_util_partition(arr, pivot, arr_size, sizeof(int), (int_cmp));
	end = clock();
	if(tmp)
	   printf("term %u, qselect returned %d\n", (unsigned)term, *(int *)tmp);
	printf("time = %f\n", (double)(end - start) / CLOCKS_PER_SEC);
	*/
	
	arr[arr_size-1] = arr_size;
	tmpNum = arr_size;
	tmp = bl_util_bsearch(&tmpNum, arr, arr_size, sizeof(int), int_cmp);
	if(tmp) {
	   printf("tmpNum = %d\tfound = %d\n", tmpNum, *(int *)tmp);
	} else {
	    printf("null returned\n");
	}
	
	for(i = 0; i < arr_size; i++)
	    printf("%d, ", arr[i]);
	printf("\n");
	
	
	return 0;
}

int int_cmp(const void *i1, const void *i2)
{
	return (*(int *)i1 > *(int *)i2) - (*(int *)i1 < *(int *)i2);
}
