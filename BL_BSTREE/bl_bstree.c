/* BL_BSTREE functions */
/*/////////////////////*/

#include "bl_bstree.h"


struct bl_bstree_t {
	size_t size;
	int (*cmp_func)(const void *, const void *);
	
	struct bl_bstree_elm {
		void *data;
		struct bl_bstree_elm *left, *right;
	} *head;
};

bl_bstree *bl_bstree_new(int (*cmp_func)(const void *, const void *))
{
	bl_bstree *bstree = malloc(sizeof(bl_bstree));
	bstree->size = 0;
	bstree->head = NULL;
	return bstree;
}

void bl_bstree_insert(bl_bstree *bstree, void *data)
{
    struct bl_bstree_elm *elm = malloc(sizeof(struct bl_bstree_elm));
    *elm = (struct bl_bstree_elm){data, NULL, NULL};
    if(!bstree->head) {
        // add as new head
        bstree->head = elm;
        return;
    }
    
    struct bl_bstree_elm *cur = bstree->head;
    while(1) {
        int cmpVal = bstree->cmp_func(data, cur->data);
        if(cmpVal <= 0) { //go left
            if(cur->left) {
                cur = cur->left;
            } else {
                cur->left = elm;
                return;
            }
        } else { //go right
            if(cur->right) {
                cur = cur->right;
            } else {
                cur->right = elm;
                return;
            }
        }
    }
}

void *bl_bstree_delete(bl_bstree *bstree, void *data)
{
    struct bl_bstree_elm *cur = bstree->head;
    struct bl_bstree_elm *prev = NULL;
    void *retVal = NULL;
    
    while(cur) {
        int cmpVal = bstree->cmp_func(data, cur->data);
        if(cmpVal < 0) { //go left
            prev = cur;
            cur = cur->left;
        } else if(cmpVal > 0) { //go right
            prev = cur;
            cur = cur->right;
        } else { //found element
            retVal = cur->data;
            
            break;
        }
    }
    return retVal;
}

struct bl_bstree_elm **get_successor(struct bl_bstree_elm *elm)
{
    struct bl_bstree_elm *cur = elm->right;
    while(cur) {
        //TODO: finish
    }
}

void *bl_bstree_find(bl_bstree *bstree, void *data)
{
    struct bl_bstree_elm *cur = bstree->head;
    
    while(cur) {
        int cmpVal = bstree->cmp_func(data, cur->data);
        if(cmpVal < 0) { //go left
            cur = cur->left;
        } else if(cmpVal > 0) {//go right
            cur = cur->right;
        } else {//found element
            break;
        }
    }
    
    if(cur) {
        return cur->data;
    } else {
        return NULL;
    }
}
