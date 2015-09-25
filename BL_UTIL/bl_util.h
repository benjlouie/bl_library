/* Utility functions */
/*///////////////////*/

#ifndef BL_UTIL_H
#define BL_Util_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void *bl_util_malloc(size_t size);
void *bl_util_calloc(size_t elements, size_t size_elms);
void *bl_util_realloc(void *orig, size_t new_size);
void bl_util_arr_set(void *base, size_t size, size_t var_size, unsigned elips_args, ...); /* FIX */
void *bl_util_qselect(void *arr, size_t nterm, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *));
void bl_util_partition(void *arr, void *pivot, size_t size, size_t var_size, int (cmp_func)(const void *, const void *));
void bl_util_char_replace(char *str, size_t size, char oldc, char newc);

#endif
