/* BL_UTIL functions */
/*///////////////////*/

#include "bl_util.h"

#define BL_UTIL_MALLOC_FAIL 5
#define BL_UTIL_CALLOC_FAIL 5
#define BL_UTIL_REALLOC_FAIL 5

void *bl_util_malloc(size_t size)
{
	void *tmp = NULL;
	int i = 0;
	do {
		if(i >= BL_UTIL_MALLOC_FAIL) {
			exit(12);
		}
		tmp = malloc(size);
		i++;
	} while(tmp == NULL);
	
	return tmp;
}

void *bl_util_calloc(size_t elements, size_t size_elms)
{
	void *tmp = NULL;
	int i = 0;
	do {
		if(i >= BL_UTIL_CALLOC_FAIL) {
			exit(12);
		}
		tmp = calloc(elements, size_elms);
		i++;
	} while(tmp == NULL);
	
	return tmp;
}

void *bl_util_realloc(void *orig, size_t new_size)
{
	void *tmp = NULL;
	int i = 0;
	do {
		if(i >= BL_UTIL_REALLOC_FAIL) {
			exit(12);
		}
		tmp = realloc(orig, new_size);
		i++;
	} while(tmp == NULL);
	
	return tmp;
}

/* make a new section for array functions? */
/* ALSO, FIX THIS, IT MESSES UP WITH CHAR */
void bl_util_arr_set(void *base, size_t size, size_t var_size, unsigned elips_args, ...)
{
	va_list listPointer;
	unsigned i = 0;
	char *chr_loop = NULL;
	
	va_start(listPointer, elips_args);
	chr_loop = malloc(sizeof(char) * elips_args);
	
	for(i = 0 ; i < elips_args; i++) {	/* get ellipsis arguments (characters) */
		chr_loop[i] = (char)va_arg(listPointer, int);
	}
	
	for(i = 0; i < size * var_size; i++) {
		*((char *)base + i) = chr_loop[i / elips_args];
	}
	
	va_end(listPointer);
}

void *bl_util_qselect(void *arr, size_t nterm, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *))
{
    #define QSELECT_SWAP(a, b)    ({ size_t _size = var_size; \
                                   char *_a = (a), *_b = (b); \
                                   do { \
                                      char _tmp = *_a; \
                                      *_a++ = *_b; \
                                      *_b++ = _tmp; \
                                   } while(--_size > 0); })
    char *base = (char *)arr;
    char *lo, *hi, *mid;
    char *start, *end;
    char *pivot_lsect, *pivot_hsect;
    char *pivot;
    int cmp;

    if(nterm > size - 1) {
        return NULL;
    }

    lo = base;
    hi = base + (size - 1) * var_size;

    do{
        /* get indexes */
        start = lo;
        end = hi;
        mid = lo + var_size * (((hi - lo) / var_size) / 2); /* (lo + hi) / 2 */

        /* median of three pivot, placed at base[hi] */
        if(cmp_func(lo, mid) > 0)
            QSELECT_SWAP(lo, mid);
        if(cmp_func(hi, mid) > 0)
            QSELECT_SWAP(mid, hi);
        else if(cmp_func(lo, hi) > 0)
            QSELECT_SWAP(lo, hi);

        pivot = hi;
        hi -= var_size;
        pivot_lsect = lo; /* one beyond section */
        pivot_hsect = hi;

        /* partition, |=P|<P|>P|=P| */
        while(lo <= hi) {
            cmp = cmp_func(lo, pivot);
            if(cmp > 0) {

                while(lo <= hi) {
                    cmp = cmp_func(hi, pivot);
                    if(cmp < 0) {
                        QSELECT_SWAP(lo, hi);
                        hi -= var_size;
                        break;
                    } else if(cmp == 0) {
                        QSELECT_SWAP(hi, pivot_hsect);
                        pivot_hsect -= var_size;
                    }
                    hi -= var_size;
                }
                /* possibly need check if(hi < start) break;  but not needed so far */
            } else if(cmp == 0) {
                QSELECT_SWAP(lo, pivot_lsect);
                pivot_lsect += var_size;
            }
            lo += var_size;
        }

        /* hi + 1 is the address gap where the pivots need to be placed */
        lo = hi;
        hi += var_size;
        /* reorient partition, |<P|=P|>P| */
        while(pivot_lsect > start) {
            pivot_lsect -= var_size;
            QSELECT_SWAP(lo, pivot_lsect);
            lo -= var_size;
        }
        while(pivot_hsect < end) {
            pivot_hsect += var_size;
            QSELECT_SWAP(hi, pivot_hsect);
            hi += var_size;
        }

        if(nterm <= (lo - base) / var_size && lo >= start) { /* in low section */
            hi = lo;
            lo = start;
        } else if(nterm >= (hi - base) / var_size && hi <= end) { /* in high section */
            lo = hi;
            hi = end;
        } else { /* in pivot section */
            return base + nterm * var_size;
        }

    } while(1);
        
    return NULL;
}

void bl_util_partition(void *arr, void *pivot, size_t size, size_t var_size, int (cmp_func)(const void *, const void *))
{
    #define PARTITION_SWAP(a, b)    ({ size_t _size = var_size; \
                                   char *_a = (a), *_b = (b); \
                                   do { \
                                      char _tmp = *_a; \
                                      *_a++ = *_b; \
                                      *_b++ = _tmp; \
                                   } while(--_size > 0); })
    char *base = (char *)arr;
    char *lo, *hi;
    char *start, *end;
    char *pivot_lsect, *pivot_hsect;
    int cmp;
    
    if(size <= 1) {
        return;
    }
    
    /* get indexes */
    lo = base;
    hi = base + (size - 1) * var_size;
    start = lo;
    end = hi;
    pivot_lsect = lo; /* one beyond section */
    pivot_hsect = hi;
    
    /* partition, |=P|<P|>P|=P| */
    while(lo <= hi) {
        cmp = cmp_func(lo, pivot);
        if(cmp > 0) {

            while(lo <= hi) {
                cmp = cmp_func(hi, pivot);
                if(cmp < 0) {
                    PARTITION_SWAP(lo, hi);
                    hi -= var_size;
                    break;
                } else if(cmp == 0) {
                    PARTITION_SWAP(hi, pivot_hsect);
                    pivot_hsect -= var_size;
                }
                hi -= var_size;
            }
        } else if(cmp == 0) {
            PARTITION_SWAP(lo, pivot_lsect);
            pivot_lsect += var_size;
        }
        lo += var_size;
    }
    
    /* hi + 1 is the address gap where the pivots need to be placed */
    lo = hi;
    hi += var_size;
    /* reorient partition, |<P|=P|>P| */
    while(pivot_lsect > start) {
        pivot_lsect -= var_size;
        PARTITION_SWAP(lo, pivot_lsect);
        lo -= var_size;
    }
    while(pivot_hsect < end) {
        pivot_hsect += var_size;
        PARTITION_SWAP(hi, pivot_hsect);
        hi += var_size;
    }
}

void bl_util_char_replace(char *str, size_t size, char oldc, char newc)
{
    size_t i = 0;
    
    for( ; i < size; i++) {
        if(str[i] == oldc)
            str[i] = newc;
    }
}




