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


/**
 * creates an empty binary search tree
 * @param cmp_func function used to compare each element of the tree
 * @note cmp_func should return <0, 0, >0 if var1 is <, =, > var2
 * @return the empty binary search tree
 */
bl_bstree *bl_bstree_new(int (*cmp_func)(const void *, const void *))
{
	bl_bstree *bstree = malloc(sizeof(bl_bstree));
	bstree->size = 0;
	bstree->cmp_func = cmp_func;
	bstree->head = NULL;
	return bstree;
}

/**
 * inserts the given data into the binary search tree
 * @param bstree the tree
 * @param data data to insert
 */
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
        if(bstree->cmp_func(data, cur->data) <= 0) { //go left
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

/**
 * removes the element equal to the givnen data from the tree
 * @param bstree the tree
 * @param data data to remove
 * @return ptr to data of deleted element, NULL if no element was found
 */
void *bl_bstree_remove(bl_bstree *bstree, void *data)
{
    struct bl_bstree_elm *cur = bstree->head;
    struct bl_bstree_elm **prev = &bstree->head;
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
            /* predecessor then successor to try and keep left/right distribution even with insert() */
            if((replace = pop_predecessor(cur))) {
                replace->left = cur->left;
                replace->right = cur->right;
            } else if((replace = pop_successor(cur))) {
                replace->left = cur->left;
                replace->right = cur->right;
            }
            *prev = replace;
            bstree->size--;
            break;
        }
    }
    free(cur);
    return retVal;
}

/**
 * helper function that pops the successor of the given element
 * @param elm element of the tree to start looking for the successor from
 * @return ptr to popped successor, NULL if no successor exists
 * @note alters the tree, helps with other element removal functions
 * @see bl_bstree_remove()
 */
struct bl_bstree_elm *pop_successor(struct bl_bstree_elm *elm)
{
    struct bl_bstree_elm *cur = elm->right;
    struct bl_bstree_elm **prev = &elm->right;
    if(cur) {
        while(cur->left) {
            prev = &cur->left;
            cur = cur->left;
        }
        *prev = cur->right;
        return cur;
    } else {
        return NULL;
    }
}

/**
 * helper function that pops the predecessor of the given element
 * @param elm element of the tree to start looking for the predecessor from
 * @return ptr to popped predecessor, NULL if no predecessor exists
 * @note alters the tree, helps with other element removal functions
 * @see bl_bstree_remove()
 */
struct bl_bstree_elm *pop_predecessor(struct bl_bstree_elm *elm)
{
    struct bl_bstree_elm *cur = elm->left;
    struct bl_bstree_elm **prev = &elm->left;
    if(cur) {
        while(cur->right) {
            prev = &cur->right;
            cur = cur->right;
        }
        *prev = cur->left;
        return cur;
    } else {
        return NULL;
    }
}

/**
 * finds the element in the tree equal to the given data
 * @param bstree the tree
 * @param data the data to look for
 * @return ptr to found data, NULL if element was not founc
 */
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
            return cur->data;
        }
    }
    return NULL;
}

/**
 * performs the given function on every element of the tree inorder
 * @param bstree the tree
 * @param extraData extra data to send the given function
 * @param func performed on each element, sent the element's data and extraData
 * @note changing the value of the data may invalidate the tree, use caution
 */
void bl_bstree_foreach_inorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    if(func) {
        foreach_inorder(bstree->head, extraData, func);
    }
}

/**
 * performs the given function on every element of the tree preorder
 * @param bstree the tree
 * @param extraData extra data to send the given function
 * @param func performed on each element, sent the element's data and extraData
 * @note changing the value of the data may invalidate the tree, use caution
 */
void bl_bstree_foreach_preorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    if(func) {
        foreach_preorder(bstree->head, extraData, func);
    }
}

/**
 * performs the given function on every element of the tree postorder
 * @param bstree the tree
 * @param extraData extra data to send the given function
 * @param func performed on each element, sent the element's data and extraData
 * @note changing the value of the data may invalidate the tree, use caution
 */
void bl_bstree_foreach_postorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    if(func) {
        foreach_postorder(bstree->head, extraData, func);
    }
}

/**
 * worker that performs the given function on every element of the tree inorder
 * @param elm the top of the subtree
 * @param extraData extra data to send the given function
 * @param func performed on each element, sent the element's data and extraData
 * @note changing the value of the data may invalidate the tree, use caution
 */
void foreach_inorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData))
{
    if(elm) {
        foreach_inorder(elm->left, extraData, func);
        func(elm->data, extraData);
        foreach_inorder(elm->right, extraData, func);
    }
}

/**
 * worker that performs the given function on every element of the tree preorder
 * @param elm the top of the subtree
 * @param extraData extra data to send the given function
 * @param func performed on each element, sent the element's data and extraData
 * @note changing the value of the data may invalidate the tree, use caution
 */
void foreach_preorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData))
{
    if(elm) {
        func(elm->data, extraData);
        foreach_preorder(elm->left, extraData, func);
        foreach_preorder(elm->right, extraData, func);
    }
}

/**
 * worker that performs the given function on every element of the tree postorder
 * @param elm the top of the subtree
 * @param extraData extra data to send the given function
 * @param func performed on each element, sent the element's data and extraData
 * @note changing the value of the data may invalidate the tree, use caution
 */
void foreach_postorder(struct bl_bstree_elm *elm, void *extraData, void (*func)(void *data, void *extraData))
{
    if(elm) {
        foreach_postorder(elm->left, extraData, func);
        foreach_postorder(elm->right, extraData, func);
        func(elm->data, extraData);
    }
}

/**
 * performs the given function on every element of the tree and removes every element
 * @param bstree the tree
 * @param extraData extra data to send the function
 * @param func performed on each element, sent the element's data and extraData
 * @note call with (bstree, NULL, NULL) to empty the tree without touching the inserted data
 * @note traverses the tree postorder
 */
void bl_bstree_foreach_remove(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData))
{
    foreach_remove(bstree->head, extraData, func);
    bstree->head = NULL;
    bstree->size = 0;
}

/**
 * worker that performs the given function on every element of the tree and removes every element
 * @param elm the top of the subtree
 * @param extraData extra data to send the function
 * @param func performed on each element, sent the element's data and extraData
 * @note traverses the tree postorder
 */
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

/**
 * frees all bstree data, does not touch inserted data
 * @param bstree the tree
 */
void bl_bstree_free(bl_bstree *bstree)
{
    foreach_remove(bstree->head, NULL, NULL);
    free(bstree);
}

/**
 * gets the number of elements in the tree
 * @param bstree the tree
 * @return the number of elements in the tree
 */
size_t bl_bstree_count(bl_bstree *bstree)
{
    return bstree->size;
}
