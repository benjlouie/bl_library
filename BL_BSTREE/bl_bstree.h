/* Binary Search Tree Functions */
/*//////////////////////////////*/
#ifndef BL_BSTREE_H
#define BL_BSTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct bl_bstree_t bl_bstree;

bl_bstree *bl_bstree_new(void);
/* void *bl_bstree_insert_(struct bl_bstree_elm **elm, void *data, void *key, int (*cmp_func)(void *, void *)) */
void bl_bstree_insert(bl_bstree *bstree, void *data, void *key, int (*cmp_func)(void *, void *));
/* struct bl_bstree_elm **bl_bstree_find_max(struct bl_bstree_elm **elm) */
/* struct bl_bstree_elm **bl_bstree_find_min(struct bl_bstree_elm **elm) */
/* void *bl_bstree_delete_(struct bl_bstree_elm **elm, void *key, int (*cmp_func)(void *, void *)) */
void *bl_bstree_delete(bl_bstree *bstree, void *key, int (*cmp_func)(void *, void *));
/* void bl_bstree_print_inorder_(struct bl_bstree_elm *elm, void (*print_func)(void *)) */
void bl_bstree_print_inorder(bl_bstree *bstree, void (*print_func)(void *));
/* void bl_bstree_print_preorder_(struct bl_bstree_elm *elm, void (*print_func)(void *)) */
void bl_bstree_print_preorder(bl_bstree *bstree, void (*print_func)(void *));
/* void bl_bstree_print_postorder_(struct bl_bstree_elm *elm, void (*print_func)(void *)) */
void bl_bstree_print_postorder(bl_bstree *bstree, void (*print_func)(void *));

#endif
