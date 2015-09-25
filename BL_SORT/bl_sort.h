/* Sort function */
/*///////////////*/
#ifndef BL_SORT_H
#define BL_SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* void bl_sort_swap(void *base, size_t var_size, size_t a, size_t b); */
void bl_sort_insertion(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *));
/* void bl_sort_insertion_extbuf(struct bl_sort_ctrl *ctrl, size_t size, size_t offset); */
/* void bl_sort_insertion_extbuf_short(struct bl_sort_ctrl_t *ctrl, size_t size, size_t offset); */
/* void bl_sort_heap_heapify(void *base, size_t size, size_t var_size, int (*cmp_func)(void *, void*)); */
void bl_sort_heap(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *));
/* void bl_sort_merge_m(struct bl_sort_ctrl *ctrl, size_t left, size_t middle, size_t right); */
/* void bl_sort_merge_(struct bl_sort_ctrl *ctrl, size_t left, size_t right); */
void bl_sort_merge(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *));
/* void *bl_sort_quick_pivot(struct bl_sort_ctrl_t *ctrl, size_t left, size_t right); */
/* void *bl_sort_quick_partition(struct bl_sort_ctrl_t *ctrl, size_t left, size_t right); */
/* void bl_sort_quick_(struct bl_sort_ctrl_t *ctrl, size_t left, size_t right); */
void bl_sort_quick(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *));

/* void bl_sort_qsort_(struct bl_sort_ctrl_t *ctrl) */
void bl_sort_qsort(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *));

/* int bl_sort_radix_maxnum(int *base, size_t size); */
void bl_sort_radix(int *base, size_t size);

#endif
