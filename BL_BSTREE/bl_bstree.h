/* Binary Search Tree Functions */
/*//////////////////////////////*/
#ifndef BL_BSTREE_H
#define BL_BSTREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct bl_bstree_t bl_bstree;

bl_bstree *bl_bstree_new(int (*cmp_func)(const void *, const void *));
void bl_bstree_insert(bl_bstree *bstree, void *data);
void *bl_bstree_remove(bl_bstree *bstree, void *data);
void *bl_bstree_find(bl_bstree *bstree, void *data);
void bl_bstree_foreach_inorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData));
void bl_bstree_foreach_preorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData));
void bl_bstree_foreach_postorder(bl_bstree *bstree, void *extraData, void (*func)(void *data, void *extraData));

#endif
