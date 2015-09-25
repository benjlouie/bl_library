/* BL_COMPARE functions */
/*//////////////////////*/

#include "bl_compare.h"


int bl_cmp_int_fast(const void *n1, const void *n2)
{
	return *(int *)n1 - *(int *)n2;
}

int bl_cmp_int_op(const void *n1, const void *n2)
{
	return (*(int *)n1 > *(int *)n2) - (*(int *)n1 < *(int *)n2);
}

int bl_cmp_float(const void *f1, const void *f2)
{
    return (*(float *)f1 > *(float *)f2) - (*(float *)f1 < *(float *)f2);
}

int bl_cmp_double(const void *d1, const void *d2)
{
    return (*(float *)d1 > *(float *)d2) - (*(float *)d1 < *(float *)d2);
}

int bl_cmp_chr_fast(const void *c1, const void *c2)
{
	return *(char *)c1 - *(char *)c2;
}

int bl_cmp_chr_op(const void *c1, const void *c2)
{
	return (*(char *)c1 > *(char *)c2) - (*(char *)c1 < *(char *)c2);
}

int bl_cmp_str(const void *s1, const void *s2)
{
	return strcmp((char *)s1, (char *)s2);
}

