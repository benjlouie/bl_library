/* Single Linked List Functions */
/*//////////////////////////////*/
#ifndef BL_SLIST_H
#define BL_SLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct bl_slist_t bl_slist; /* IDEA!! make it a list of unique values(with count) with a list of modes with that value off each of those nodes */

bl_slist *bl_slist_new(void);
void bl_slist_push(bl_slist *slist, void *data);
void bl_slist_add_sorted(bl_slist *slist, void *data, int (*cmp_func)(void *, void *));
void *bl_slist_pop(bl_slist *slist);
void *bl_slist_del(bl_slist *slist, void *data, int (*cmp_func)(void *, void *));
void **bl_slist_del_mult(bl_slist *slist, void *data, int (*cmp_func)(void *, void *));
void *bl_slist_peek(bl_slist *slist);
void *bl_slist_find(bl_slist *slist, void *data, int (*cmp_func)(void *, void *));
size_t bl_slist_size(bl_slist *slist);
void bl_slist_sort(bl_slist *slist, int (*cmp_func)(void *, void *));
void bl_slist_free(bl_slist **slist, void (*free_func)(void *));
void bl_slist_free_nodes(bl_slist *slist, void (*free_func)(void *));
void bl_slist_print(bl_slist *slist, void (print_func)(void *));

#endif
