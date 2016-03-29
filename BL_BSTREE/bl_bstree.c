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
void *bl_bstree_delete(bl_bstree *bstree, void *data);
struct bl_bstree_elm *pop_successor(struct bl_bstree_elm *elm);
struct bl_bstree_elm *pop_predecessor(struct bl_bstree_elm *elm);
void *bl_bstree_find(bl_bstree *bstree, void *data);

void bl_bstree_print(bl_bstree *bstree);
void bl_bstree_print_inorder(struct bl_bstree_elm *elm);
void bl_bstree_print_preorder(struct bl_bstree_elm *elm);
void bl_bstree_print_postorder(struct bl_bstree_elm *elm);


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


//TODO: remove debug print funciton, replace with foreach_inorder/preorder/postorder function
void bl_bstree_print(bl_bstree *bstree)
{
    bl_bstree_print_inorder(bstree->head);
    printf("\n");
    bl_bstree_print_preorder(bstree->head);
    printf("\n");
    bl_bstree_print_postorder(bstree->head);
    printf("\n");
}

void bl_bstree_print_inorder(struct bl_bstree_elm *elm)
{
    if(elm) {
        bl_bstree_print_inorder(elm->left);
        printf("%d, ", *(int *)elm->data);
        bl_bstree_print_inorder(elm->right);
    }
}
void bl_bstree_print_preorder(struct bl_bstree_elm *elm)
{
    if(elm) {
        printf("%d, ", *(int *)elm->data);
        bl_bstree_print_preorder(elm->left);
        bl_bstree_print_preorder(elm->right);
    }
}
void bl_bstree_print_postorder(struct bl_bstree_elm *elm)
{
    if(elm) {
        bl_bstree_print_postorder(elm->left);
        bl_bstree_print_postorder(elm->right);
        printf("%d, ", *(int *)elm->data);
    }
}
