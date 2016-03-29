#include <stdio.h>
#include "bl_bstree.h"

int cmp_int(const void *a, const void *b);

int main(void)
{
	int arr[7] = {10, 5, 20, 2, 7, 15, 25};
	
	bl_bstree *tree = bl_bstree_new(cmp_int);
	
	for(int i = 0; i < 7; i++) {
	    bl_bstree_insert(tree, &arr[i]);
	}
	
	bl_bstree_print(tree);
	printf("\n");
	
	bl_bstree_delete(tree, &arr[0]);
	bl_bstree_insert(tree, &arr[0]);
	
	bl_bstree_print(tree);
	
	return 0;
}

int cmp_int(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}
