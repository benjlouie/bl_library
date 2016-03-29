#include <stdio.h>
#include "bl_bstree.h"

int cmp_int(const void *a, const void *b);
void print_tree(bl_bstree *bstree);
void print_int(void *data, void *extraData);

int main(void)
{
	int arr[7] = {10, 5, 20, 2, 7, 15, 25};
	
	bl_bstree *tree = bl_bstree_new(cmp_int);
	
	for(int i = 0; i < 7; i++) {
	    bl_bstree_insert(tree, &arr[i]);
	}
	
	print_tree(tree);
	printf("\n");
	
	bl_bstree_remove(tree, &arr[0]);
	bl_bstree_remove(tree, &arr[5]);
	
	print_tree(tree);
	
	return 0;
}

int cmp_int(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

void print_tree(bl_bstree *bstree)
{
    bl_bstree_foreach_inorder(bstree, NULL, print_int);
    printf("\n");
    bl_bstree_foreach_preorder(bstree, NULL, print_int);
    printf("\n");
    bl_bstree_foreach_postorder(bstree, NULL, print_int);
    printf("\n");
}

void print_int(void *data, void *extraData)
{
    printf("%d, ", *(int *)data);
}
