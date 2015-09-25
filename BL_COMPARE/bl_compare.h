/* Compare functions */
/*///////////////////*/
#ifndef BL_COMPARE_H
#define BL_COMPARE_H

#include <stdio.h>
#include <strings.h>

int bl_cmp_int_fast(const void *n1, const void *n2);
int bl_cmp_int_op(const void *n1, const void *n2);
int bl_cmp_float(const void *f1, const void *f2);
int bl_cmp_double(const void *d1, const void *d2);
int bl_cmp_chr_fast(const void *c1, const void *c2);
int bl_cmp_chr_op(const void *c1, const void *c2);
int bl_cmp_str(const void *s1, const void *s2);

#endif
