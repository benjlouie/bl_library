#include <stdio.h>
#include <time.h>
#include "bl_sort.h"

void int_print(void *i);
int int_cmp(const void *i1, const void *i2);
void int_del(void *num);


int main(void)
{
	size_t arr_size = 10000000;
	int *test = NULL;
	unsigned i = 0;
	clock_t start, end;

	test = malloc(sizeof(int) * arr_size);
	
	srand(time(NULL));
	for(i = 0; i < arr_size; i++) {
		test[i] = rand() + rand() % 1000 + rand() * 1000000;
	}

	/*for(i = 0; i < arr_size; i++) {
		printf("%d, ", test[i]);
	}
	printf("\n");*/

	start = clock();
	//bl_sort_insertion(test, arr_size, sizeof(int), (int_cmp));
	//bl_sort_heap((void *)test, arr_size, sizeof(int), (int_cmp));
    //bl_sort_merge((void *)test, arr_size, sizeof(int), (int_cmp));
    bl_sort_nrmerge((void *)test, arr_size, sizeof(int), (int_cmp));
	//bl_sort_quick((void *)test, arr_size, sizeof(int), (int_cmp));
	//bl_sort_qsort((void *)test, arr_size, sizeof(int), (int_cmp));
	//bl_sort_radix(test, arr_size);
	//qsort(test, arr_size, sizeof(int), (int_cmp));
	end = clock();
	printf("time = %f\n", (float)(end - start) / CLOCKS_PER_SEC);
	
	for(i = 1; i < arr_size; i++) {
	    if(test[i - 1] > test[i])
            printf("NOT SORTED\n");
    }

	/*for(i = 0; i < arr_size; i++) {
		printf("%d, ", test[i]);
	}
	printf("\n");*/

	free(test);

	return 0;
}

void int_print(void *i)
{
	printf("%d\n", *(int *)i);
}

int int_cmp(const void *i1, const void *i2)
{
	return (*(int *)i1 > *(int *)i2) - (*(int *)i1 < *(int *)i2);
}

void int_del(void *num)
{
	free(num);
}
