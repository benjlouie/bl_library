/* BL_BSTREE functions */
/*/////////////////////*/

#include "bl_bstree.h"


struct bl_bstree_t {
	size_t size;
	
	struct bl_bstree_elm {
		void *data;
		void *key;
		struct bl_bstree_elm *left, *right;
	} *head;
};

bl_bstree *bl_bstree_new(void)
{
	bl_bstree *bstree = malloc(sizeof(bl_bstree));
	bstree->size = 0;
	bstree->head = NULL;
	return bstree;
}

void *bl_bstree_insert_(struct bl_bstree_elm **elm, void *data, void *key, int (*cmp_func)(void *, void *))
{
	if(!(*elm)) {
		*elm = malloc(sizeof(struct bl_bstree_elm));
		(*elm)->data = data;
		(*elm)->key = key;
		(*elm)->left = NULL;
		(*elm)->right = NULL;
		return *elm;
	} else {
		if((cmp_func)(key, (*elm)->key) < 0) { /* less -> left */
			return bl_bstree_insert_(&(*elm)->left, data, key, (cmp_func));
		} else if((cmp_func)(key, (*elm)->key) > 0) { /* greater -> right */
			return bl_bstree_insert_(&(*elm)->right, data, key, (cmp_func));
		} else {
			return NULL;
		}
	}
}

void bl_bstree_insert(bl_bstree *bstree, void *data, void *key, int (*cmp_func)(void *, void *))
{
	assert(bstree);
	
	if(bl_bstree_insert_(&bstree->head, data, key, (cmp_func))) {
		bstree->size++;
	}
}

struct bl_bstree_elm **bl_bstree_find_max(struct bl_bstree_elm **elm)
{
	if((*elm)->right == NULL) {
		return elm;
	}
	return bl_bstree_find_max(&(*elm)->right);
}

struct bl_bstree_elm **bl_bstree_find_min(struct bl_bstree_elm **elm)
{
	if((*elm)->left == NULL) {
		return elm;
	}
	return bl_bstree_find_min(&(*elm)->left);
}

struct bl_bstree_elm **bl_bstree_find_node(struct bl_bstree_elm **elm, void *key, int (*key_cmp)(void *, void *))
{
	if(!(*elm)) {
		return NULL;
	}
	
	if(key_cmp((*elm)->key, key) < 0) {
		return bl_bstree_find_node(&((*elm)->left), key, (key_cmp));
	} else if(key_cmp((*elm)->key, key) > 0) {
		return bl_bstree_find_node(&((*elm)->right), key, (key_cmp));
	} else {
		return elm;
	}
}

void *bl_bstree_delete_(struct bl_bstree_elm **elm, void *key, int (*cmp_func)(void *, void *)) /* find a way to free the keys. give back? delete? */
{
	void *data_tmp;
	struct bl_bstree_elm *tmp = NULL, **tmp2 = NULL;
	
	if(!(*elm)) {
		return NULL;
	}
	if((cmp_func)(key, (*elm)->key) < 0) {
		return bl_bstree_delete_(&(*elm)->left, key, (cmp_func)); /* check returns */
	} else if((cmp_func)(key, (*elm)->key) > 0) {
		return bl_bstree_delete_(&(*elm)->right, key, (cmp_func));
	} else {		/* What about when there is a count >1? return void? return data? don't use count? */
		data_tmp = (*elm)->data;
		if((*elm)->left == NULL) {
			if((*elm)->right == NULL) {
				free(*elm);
				*elm = NULL;
				/* no nodes */
			} else {
				tmp = (*elm);
				*elm = (*elm)->right; /* check this */
				free(tmp);
				/* right only */
			}
		} else {
			if((*elm)->right == NULL) {
				tmp = (*elm);
				*elm = (*elm)->left;/* check this */
				free(tmp);
				/* left only */
			} else {
				/* tmp = *elm; */
				tmp2 = bl_bstree_find_max(&(*elm)->left);
				if((*tmp2)->left != NULL) {
					(*elm)->data = (*tmp2)->data;
					(*elm)->key = (*tmp2)->key;
					tmp = *tmp2;
					tmp2 = &(*tmp2)->left; /* check this */
					free(tmp);
					/* connect */
				} else {
					printf("test1\n");	/*////////////// revise so it works, something to do with find_max() ///////*/
					(*elm)->data = (*tmp2)->data;
					(*elm)->key = (*tmp2)->key;
					printf("test2\n");
					free(tmp2);
					printf("test3\n");
					/* no connect */
				}
				/* both nodes */
			}
		}
		return data_tmp;
	}

	return NULL;
}

void *bl_bstree_delete(bl_bstree *bstree, void *key, int (*cmp_func)(void *, void *))
{
	void *tmp = NULL;
	
	assert(bstree);
	tmp = bl_bstree_delete_(&bstree->head, key, (cmp_func));
	if(tmp) {
		bstree->size--;
	}
	return tmp;
}

void bl_bstree_print_inorder_(struct bl_bstree_elm *elm, void (*print_func)(void *))	/* maybe make the print have data and key? */
{
	if(!elm) {
		return;
	}
	bl_bstree_print_inorder_(elm->left, (print_func));
	(print_func)(elm->data);
	bl_bstree_print_inorder_(elm->right, (print_func));
}

void bl_bstree_print_inorder(bl_bstree *bstree, void (*print_func)(void *))
{
	bl_bstree_print_inorder_(bstree->head, (print_func));
}

void bl_bstree_print_preorder_(struct bl_bstree_elm *elm, void (*print_func)(void *))
{
	if(!elm) {
		return;
	}
	(print_func)(elm->data);
	bl_bstree_print_preorder_(elm->left, (print_func));
	bl_bstree_print_preorder_(elm->right, (print_func));
}

void bl_bstree_print_preorder(bl_bstree *bstree, void (*print_func)(void *))
{
	bl_bstree_print_preorder_(bstree->head, (print_func));
}

void bl_bstree_print_postorder_(struct bl_bstree_elm *elm, void (*print_func)(void *))
{
	if(!elm) {
		return;
	}
	bl_bstree_print_postorder_(elm->left, (print_func));
	bl_bstree_print_postorder_(elm->right, (print_func));
	(print_func)(elm->data);
}

void bl_bstree_print_postorder(bl_bstree *bstree, void (*print_func)(void *))
{
	bl_bstree_print_postorder_(bstree->head, (print_func));
}
