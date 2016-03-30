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

bl_bstree *bl_bstree_new(int (*cmp_func)(const void *, const void *));
void bl_bstree_insert(bl_bstree *bstree, void *data);
void *bl_bstree_remove(bl_bstree *bstree, void *data);
struct bl_bstree_elm *pop_successor(struct bl_bstree_elm *elm);
struct bl_bstree_elm *pop_predecessor(struct bl_bstree_elm *elm);
void *bl_bstree_find(bl_bstree *bstree, void *data);
void bl_bstree_foreach_inorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData));
void bl_bstree_foreach_preorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData));
void bl_bstree_foreach_postorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData));
void foreach_inorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData));
void foreach_preorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData));
void foreach_postorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData));
void bl_bstree_foreach_remove(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData));
void foreach_remove(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData));
void bl_bstree_free(bl_bstree *bstree);
size_t bl_bstree_count(bl_bstree *bstree);


bl_bstree *bl_bstree_new(int (*cmp_func)(const void *, const void *))
{
	bl_bstree *bstree = malloc(sizeof(bl_bstree));
	bstree->size = 0;
	bstree->cmp_func = cmp_func;
	bstree->head = NULL;
	return bstree;
}

void bl_bstree_insert(bl_bstree *bstree, void *data)
{
    struct bl_bstree_elm *elm = malloc(sizeof(struct bl_bstree_elm));
    *elm = (struct bl_bstree_elm){data, NULL, NULL};
    bstree->size++;
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

void *bl_bstree_remove(bl_bstree *bstree, void *data)
{
    struct bl_bstree_elm *cur = bstree->head;
    struct bl_bstree_elm **prev = NULL;
    void *retVal = NULL;
    
    while(cur) {
        int cmpVal = bstree->cmp_func(data, cur->data);
        if(cmpVal < 0) { //left
            prev = &cur->left;
            cur = cur->left;
        } else if(cmpVal > 0) { //right
            prev = &cur->right;
            cur = cur->right;
        } else { //found element
            retVal = cur->data;
            struct bl_bstree_elm *replace = NULL;
            if((replace = pop_successor(cur))) {
                replace->left = cur->left;
                replace->right = cur->right;
                if(prev) {
                    *prev = replace;
                } else {
                    bstree->head = replace;
                }
            } else if((replace = pop_predecessor(cur))) {
                replace->left = cur->left;
                replace->right = cur->right;
                if(prev) {
                    *prev = replace;
                } else {
                    bstree->head = replace;
                }
            } else {
                if(prev) {
                    *prev = NULL;
                } else { //head, no replacement
                    bstree->head = NULL;
                }
            }
            bstree->size--;
            break;
        }
    }
    free(cur);
    return retVal;
}

struct bl_bstree_elm *pop_successor(struct bl_bstree_elm *elm)
{
    struct bl_bstree_elm *cur = elm->right;
    struct bl_bstree_elm **prev = &elm->right;
    if(cur) {
        while(cur->left) {
            prev = &cur->left;
            cur = cur->left;
        }
        if(prev) {
            *prev = cur->right;
        }
        return cur;
    } else {
        return NULL;
    }
}

struct bl_bstree_elm *pop_predecessor(struct bl_bstree_elm *elm)
{
    struct bl_bstree_elm *cur = elm->left;
    struct bl_bstree_elm **prev = &elm->left;
    if(cur) {
        while(cur->right) {
            prev = &cur->right;
            cur = cur->right;
        }
        if(prev) {
            *prev = cur->left;
        }
        return cur;
    } else {
        return NULL;
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

void bl_bstree_foreach_inorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    if(func) {
        foreach_inorder(bstree->head, extraData, func);
    }
}

void bl_bstree_foreach_preorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    if(func) {
        foreach_preorder(bstree->head, extraData, func);
    }
}

void bl_bstree_foreach_postorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    if(func) {
        foreach_postorder(bstree->head, extraData, func);
    }
}

void foreach_inorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData))
{
    if(elm) {
        foreach_inorder(elm->left, extraData, func);
        func(elm->data, extraData);
        foreach_inorder(elm->right, extraData, func);
    }
}

void foreach_preorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData))
{
    if(elm) {
        func(elm->data, extraData);
        foreach_preorder(elm->left, extraData, func);
        foreach_preorder(elm->right, extraData, func);
    }
}

void foreach_postorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData))
{
    if(elm) {
        foreach_postorder(elm->left, extraData, func);
        foreach_postorder(elm->right, extraData, func);
        func(elm->data, extraData);
    }
}

void bl_bstree_foreach_remove(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    foreach_remove(bstree->head, extraData, func);
    bstree->head = NULL;
    bstree->size = 0;
}

void foreach_remove(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData))
{
    // postorder removal
    if(elm) {
        foreach_remove(elm->left, extraData, func);
        foreach_remove(elm->right, extraData, func);
        if(func) {
            func(elm->data, extraData);
        }
        free(elm);
    }
}

void bl_bstree_free(bl_bstree *bstree)
{
    foreach_remove(bstree->head, NULL, NULL);
    free(bstree);
}

size_t bl_bstree_count(bl_bstree *bstree)
{
    return bstree->size;
}
