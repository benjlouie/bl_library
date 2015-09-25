/* BL_SORT functions */
/*///////////////////*/


/*///// Integrate structure into all the functions /////*/
/*///// Use pointer arithmetic instead of mult in all functions /////*/

#include "bl_sort.h"

#define BL_SORT_MERGE_THRESHOLD 24 /* 24,28 is best with my limited tests */
#define BL_SORT_QUICK_PIVOT_THRESHOLD 5 /* number of samples for median pivot, keep low */
#define BL_SORT_QUICK_THRESHOLD 16 /* 8, 16 is best with my limited tests */
#define BL_SORT_QSORT_THRESHOLD 1

struct bl_sort_ctrl_t {
    void *base;
    size_t size;
    size_t var_size;
    int (*cmp_func)(const void *, const void *);
    void *ext_buf;
    void *ext_vbuf;
};


void bl_sort_swap(void *base, size_t var_size, size_t a, size_t b)    /*//////// optimize!!! /////////*/
{
    char c;
    size_t i = 0;
    a = a * var_size;
    b = b * var_size;
    
    for(i = 0; i < var_size; i++) {
        c = *((char *)base + a + i);
        *((char *)base + a + i) = *((char *)base + b + i);
        *((char *)base + b + i) = c;
    }
}

void bl_sort_swap_extbuf(struct bl_sort_ctrl_t *ctrl, void *a, void *b)
{
    if(a != b) {
        memcpy(ctrl->ext_vbuf, b, ctrl->var_size);
        memcpy(b, a, ctrl->var_size);
        memcpy(a, ctrl->ext_vbuf, ctrl->var_size);
    }
}

/* add test for sequential elements of the same value */
void bl_sort_insertion(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *))
{
    size_t i = 0, j = 0;
    char *tmp = NULL;
    tmp = malloc(var_size);
    
    for(i = 1; i < size; i++) {
        j = i;
        
        while(j > 0 && cmp_func((char *)base + (j - 1) * var_size, (char *)base + i * var_size) > 0) {
            j--;
        }
        if(j != i) {    /* there is stuff to move */
            memcpy(tmp, (char *)base + i * var_size, var_size);
            memmove((char *)base + (j + 1) * var_size, (char *)base + j * var_size, (i - j) * var_size);
            memcpy((char *)base + j * var_size, tmp, var_size);
        }
    }
    free(tmp);
}

void bl_sort_insertion_extbuf(struct bl_sort_ctrl_t *ctrl, size_t size, size_t offset)
{
    size_t i = 0, j = 0;
    void *base = (char *)ctrl->base + offset * ctrl->var_size;
    size_t var_size = ctrl->var_size;
    void *ext_vbuf = ctrl->ext_vbuf;
    
    for(i = 1; i < size; i++) {
        j = i;
        
        while(j > 0 && ctrl->cmp_func((char *)base + (j - 1) * var_size, (char *)base + i * var_size) > 0) {
            j--;
        }
        if(j != i) {    /* there is stuff to move */
            memcpy(ext_vbuf, (char *)base + i * var_size, var_size);
            memmove((char *)base + (j + 1) * var_size, (char *)base + j * var_size, (i - j) * var_size);
            memcpy((char *)base + j * var_size, ext_vbuf, var_size);
        }
    }
}

void bl_sort_insertion_extbuf_short(struct bl_sort_ctrl_t *ctrl, size_t size, size_t offset)
{
    #define INSERTION_SHORT_MOVE(d, s) ({ do{ size_t _size = var_size; \
                                   char *_d = (d), *_s = (s); \
                                   do { \
                                      *_d++ = *_s++; \
                                   } while(--_size > 0); }while(0); })
    size_t i = 0, j = 0;
    char *base = (char *)ctrl->base + offset * ctrl->var_size;
    size_t var_size = ctrl->var_size;
    void *ext_vbuf = ctrl->ext_vbuf;

    for(i = 1; i < size; i++) {
        j = i;
        INSERTION_SHORT_MOVE(ext_vbuf, base + i * var_size);
        while(j > 0 && ctrl->cmp_func(base + (j - 1) * var_size, ext_vbuf) > 0) {
            INSERTION_SHORT_MOVE(base + j * var_size, base + (j - 1) * var_size);
            j--;
        }
        INSERTION_SHORT_MOVE(base + j * var_size, ext_vbuf);
    }
}

void bl_sort_heap_heapify(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void*))
{
    size_t i = 0, j = 0;
    size_t parent = 0;
    
    for(i = 1; i < size; i++) {
        j = i;
        while(j > 0 && (cmp_func)((char *)base + (parent = (j - 1) / 2) * var_size, (char *)base + j * var_size) < 0) {
            bl_sort_swap(base, var_size, j, parent);
            j = parent;
        }
    }
}

void bl_sort_heap(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *))
{
    size_t end = size - 1;
    size_t left = 0, right = 0;
    size_t i = 0;
    
    if(size < 2) {
        return;
    }
    
    bl_sort_heap_heapify(base, size, var_size, (cmp_func));
    
    for(end = size - 1; end > 0; end--) {
        bl_sort_swap(base, var_size, 0, end);
        i = 0;
        
        while(1){
            left = i * 2 + 1;
            right = i * 2 + 2;
            if(right < end && (cmp_func)((char *)base + right * var_size, (char *)base + left * var_size) >= 0) {
                if((cmp_func)((char *)base + i * var_size, (char *)base + right * var_size) < 0) {
                    bl_sort_swap(base, var_size, i, right);
                    i = right;
                } else {
                    break;
                }
            } else if(left < end && (cmp_func)((char *)base + i * var_size, (char *)base + left * var_size) < 0) {
                bl_sort_swap(base, var_size, i, left);
                i = left;
            } else {
                break;
            }
        }
    }
}

/* add test for sequential elements of the same value */
void bl_sort_merge_m(struct bl_sort_ctrl_t *ctrl, size_t left, size_t middle, size_t right)
{
    size_t var_size = ctrl->var_size;
    char *base = (char *)ctrl->base + left * var_size;                     /* start of base arr */
    char *base_rhalf = (char *)ctrl->base + (middle + 1) * var_size;       /* start of right half of arr */
    char *base_rhalf_max = (char *)ctrl->base + right * var_size;          /* max address for second half arr */
    char *ext_buf = (char *)ctrl->ext_buf;                                 /* start of external arr */
    char *ext_buf_max = (char *)ctrl->ext_buf + ((middle - left) + 1) * var_size;  /* max addr of ext arr */
    
    /* put the first half of base array into ext_buf */
    memcpy(ext_buf, base, ((middle - left) + 1) * var_size);
    
    /* merge ext_buf and second half of base into base array */
    while(ext_buf < ext_buf_max && base_rhalf <= base_rhalf_max) {
        if(ctrl->cmp_func(ext_buf, base_rhalf) <= 0) {
            memcpy(base, ext_buf, var_size);
            ext_buf += var_size;
        } else {
            memcpy(base, base_rhalf, var_size);
            base_rhalf += var_size;
        }
        base += var_size;
    }
    
    if(ext_buf < ext_buf_max) { /* dump remaining first half into base */
        memcpy(base, ext_buf, (size_t)(ext_buf_max - ext_buf));
    }
}

/*//////////change the name "right" to "end" or something, try and optimize this, http://stackoverflow.com/questions/18628152/ /////////*/
void bl_sort_merge_(struct bl_sort_ctrl_t *ctrl, size_t left, size_t right) {
    size_t middle;
    if((right - left) + 1 <= BL_SORT_MERGE_THRESHOLD) {
        /*bl_sort_insertion((char *)ctrl->base + left * ctrl->var_size, right - left + 1, ctrl->var_size, ctrl->cmp_func);*/
        bl_sort_insertion_extbuf(ctrl, right - left + 1, left);
    } else {
        middle = ((right - left) >> 1) + left; /* same as (l+r)/2, avoids overflow */
        /* recursively split the list */
        bl_sort_merge_(ctrl, left, middle);
        bl_sort_merge_(ctrl, middle + 1, right);
        bl_sort_merge_m(ctrl, left, middle, right); /* merge the lists */
    }
}

void bl_sort_merge(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *))
{
    struct bl_sort_ctrl_t ctrl;
    
    if(size > 1) {
        ctrl.base = base;
        ctrl.size = size;
        ctrl.var_size = var_size;
        ctrl.cmp_func = cmp_func;
        ctrl.ext_buf = malloc((size / 2) * var_size);    /* (size + 1) / 2?? there aren't any problems so far */
        ctrl.ext_vbuf = malloc(var_size);
        bl_sort_merge_(&ctrl, 0, size - 1);
        free(ctrl.ext_buf);
        free(ctrl.ext_vbuf);
    }
}
    #define NRMERGE_SWAP(a, b)    ({ do{ size_t _size = var_size; \
                                   char *_a = (a), *_b = (b); \
                                   do { \
                                      char _tmp = *_a; \
                                      *_a++ = *_b; \
                                      *_b++ = _tmp; \
                                   } while(--_size > 0);  }while(0); })
void bl_sort_nrmerge_m(char *base, size_t size, size_t var_size, size_t index, size_t block_size, void *buffer, int (*cmp_func)(const void *, const void *))
{
    size_t merge_block = 2 * block_size;
    char *end;

    if(size - index <= block_size) {
        return;
    }

    if(size < index + merge_block) {
        end = base + (size * var_size);
    } else {
        end = base + ((index + merge_block) * var_size);
    }
    /////////Merge here//////
    char *i, *j, *k, *buf_end;
    memcpy(buffer, base + (index * var_size), block_size * var_size); // fill buffer
    i = base + (index * var_size);
    j = buffer;
    k = base + ((index + block_size) * var_size);
    buf_end = buffer + (block_size * var_size);

    for(; j < buf_end && k < end; i += var_size) {
        if(cmp_func(j, k) < 0) {
            NRMERGE_SWAP(i, j);
            j += var_size;
        } else {
            NRMERGE_SWAP(i, k);
            k += var_size;
        }
    }
    while(j < buf_end) {
        NRMERGE_SWAP(i, j);
        i += var_size; j += var_size;
    }
}

void bl_sort_nrmerge(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *))
{
    size_t block_size = 0;
    size_t merge_block = 0;
    size_t i = 0;
    void *buffer = NULL;

    if(size <= 2) {
        if(size == 2) {
            if(cmp_func(base, (char *)base + var_size) > 0) {
                NRMERGE_SWAP(base, base + var_size);
            }
        }
        return;
    }

    //// size needs to be 2^something, find best one ////
    buffer = malloc(size * var_size);

    for(block_size = 1, merge_block = 2; merge_block < size; block_size *= 2) {
        merge_block = 2 * block_size;
        for(i = 0; i + merge_block < size; i += merge_block) {
            bl_sort_nrmerge_m(base, size, var_size, i, block_size, buffer, (cmp_func));
        }
        bl_sort_nrmerge_m(base, size, var_size, i, block_size, buffer, (cmp_func));
    }

    free(buffer);
}

/*///// FIX /////*/
void *bl_sort_quick_pivot(struct bl_sort_ctrl_t *ctrl, size_t left, size_t right)
{
    void *arr[BL_SORT_QUICK_PIVOT_THRESHOLD];
    char *base = (char *)ctrl->base + left * ctrl->var_size;
    char *base_max = (char *)ctrl->base + right * ctrl->var_size;
    size_t inc = (((right - left) + 1) / (BL_SORT_QUICK_PIVOT_THRESHOLD - 1)) * ctrl->var_size;
    size_t i = 0;
    /*unsigned tmp = 0;*/
    
    /*///// there should be a way to set it up correctly /////*/
    for(i = 0; i < BL_SORT_QUICK_PIVOT_THRESHOLD - 1; i++, base += inc) {
        arr[i] = base;
    }
    arr[i++] = base_max;
/*
    for(tmp = 0; tmp < i; tmp++) {
        printf("%d\n", *(int *)arr[tmp]);
    }
    bl_sort_insertion(arr, i, sizeof(void *), ctrl->cmp_func);
    printf("\n");
    for(tmp = 0; tmp < i; tmp++) {
        printf("%d\n", *(int *)arr[tmp]);
    }
*/
 /* printf("pivot = %d\n", *(int *)arr[i / 2]); */
    
    return arr[i / 2];
}

/*///// change so functions accepts a pivot value as a parameter /////*/
/*///// tighten up the loops /////*/
void *bl_sort_quick_partition(struct bl_sort_ctrl_t *ctrl, size_t left, size_t right)
{
    size_t var_size = ctrl->var_size;
    char *base = (char *)ctrl->base;
    char *base_max = base + right * var_size;
    char *lo = base + left * var_size;
    char *hi = base + (right - 1) * var_size; /* one below where the pivot will be */
    char *pivot_part = base + right * var_size;
    char *pivot = bl_sort_quick_pivot(ctrl, left, right);
    int cmp = 0;
    
    bl_sort_swap_extbuf(ctrl, pivot, pivot_part); /* move pivot to end */
    pivot = pivot_part;
    /* partition arr into 3 sections, less-high-pivot */
    while(lo <= hi) {
        cmp = ctrl->cmp_func(lo, pivot);
        if(cmp > 0) { /* low end is higher than pivot */
            while(lo < hi) {
                cmp = ctrl->cmp_func(hi, pivot);
                if(cmp < 0) { /* high val is less than pivot */
                    bl_sort_swap_extbuf(ctrl, lo, hi);
                    hi -= var_size;
                    break;
                } else if(cmp == 0) { /* high end equal to pivot */
                    pivot_part -= var_size;
                    if(hi != pivot_part) {
                        bl_sort_swap_extbuf(ctrl, hi, pivot_part);
                    } else {
                        hi -= var_size;
                    }
                    continue;
                }
                hi -= var_size;
            }
        } else if(cmp == 0) { /* low end equal to pivot */
            pivot_part -= var_size;
            bl_sort_swap_extbuf(ctrl, lo, pivot_part); /*///// should be a check if pivot_part is also a pivot or something /////*/
            if(hi == pivot_part) {
                hi -= var_size;
            }
            continue;
        }
        lo += var_size;
    }
    if(hi < base + left * var_size || ctrl->cmp_func(hi, pivot) < 0) {
        hi += var_size;
    }
    
    lo = hi;
    pivot = base_max;
    while(hi != pivot_part && pivot >= pivot_part) { /* put pivot section in the middle */
        bl_sort_swap_extbuf(ctrl, hi, pivot);
        hi += var_size;
        pivot -= var_size;
    }
    if(hi <= pivot && hi >= pivot_part) { /* place hi at end of pivot section */
        hi = pivot + var_size;
    }
    
    ctrl->ext_buf = hi; /* ext_buf isn't used */
    return lo; /* return left */
}

void bl_sort_quick_(struct bl_sort_ctrl_t *ctrl, size_t left, size_t right)
{
    char *lo = NULL, *hi = NULL;
    char *base = ctrl->base;
    size_t var_size = ctrl->var_size;
    char *base_max = base + (right + 1) * var_size; /* addr after last var */
    size_t lo_size = 0, hi_size = 0;
    
    if((right - left) + 1 <= BL_SORT_QUICK_THRESHOLD) {
        return;
    }
    /* partition, get left and right */
    lo = bl_sort_quick_partition(ctrl, left, right); /* low */
    hi = ctrl->ext_buf; /* high, from partition() */
    lo_size = (lo - (base + left * var_size)) / var_size;
    hi_size = (base_max - hi) / var_size;
    
    /*///// CHECK THIS, change it to be sure of tail call /////*/
    if(lo_size > hi_size) {
        bl_sort_quick_(ctrl, right - hi_size + 1, right);
        bl_sort_quick_(ctrl, left, left + lo_size - 1);
    } else {
        bl_sort_quick_(ctrl, left, left + lo_size - 1);
        bl_sort_quick_(ctrl, right - hi_size + 1, right);
    }
}

void bl_sort_quick(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *))
{
    struct bl_sort_ctrl_t ctrl;
    
    if(size > 1) {
        ctrl.base = base;
        ctrl.size = size;
        ctrl.var_size = var_size;
        ctrl.cmp_func = cmp_func;
        ctrl.ext_vbuf = malloc(var_size);
        bl_sort_quick_(&ctrl, 0, size - 1);
        bl_sort_insertion_extbuf(&ctrl, size, 0);
        free(ctrl.ext_vbuf);
    }
}

void bl_sort_qsort_(struct bl_sort_ctrl_t *ctrl)
{
    #define QSORT_PUSH(s, e)    ({ do{ qsort_stack[qsort_stack_index++] = (s); qsort_stack[qsort_stack_index++] = (e); }while(0); })
    #define QSORT_POP           ({ qsort_stack[--qsort_stack_index]; }) /* will get end first */
    #define QSORT_SWAP(a, b)    ({ do{ size_t _size = var_size; \
                                   char *_a = (a), *_b = (b); \
                                   do { \
                                      char _tmp = *_a; \
                                      *_a++ = *_b; \
                                      *_b++ = _tmp; \
                                   } while(--_size > 0);  }while(0); })
    char *base = (char *)ctrl->base;
    char *qsort_stack[128]; /* 2 * log_2(2^64) * 8 bytes = 1024 bytes */
    unsigned qsort_stack_index; /* starts with whole array initially */
    size_t var_size = ctrl->var_size;
    char *lo, *hi, *mid;
    char *start, *end;
    char *pivot_lsect, *pivot_hsect;
    char *pivot;
    size_t lsize, hsize;
    int cmp;
    /* initialize stack */
    qsort_stack_index = 2;
    qsort_stack[0] = base;
    qsort_stack[1] = base + (ctrl->size - 1) * var_size;
    
    /* partition loop */
    do {
        /* get indexes */
        hi = QSORT_POP;
        lo = QSORT_POP;
        start = lo;
        end = hi;
        mid = lo + var_size * (((hi - lo) / var_size) / 2); /* (lo + hi) / 2 */

        /* median of three pivot, placed at base[hi] */
        if(ctrl->cmp_func(lo, mid) > 0)
            QSORT_SWAP(lo, mid);
        if(ctrl->cmp_func(hi, mid) > 0)
            QSORT_SWAP(mid, hi);
         else if(ctrl->cmp_func(lo, hi) > 0)
            QSORT_SWAP(lo, hi);

        pivot = hi;
        hi -= var_size;
        pivot_lsect = lo; /* one beyond section */
        pivot_hsect = hi;

        /* partition, |=P|<P|>P|=P| */
        while(lo <= hi) {
            cmp = ctrl->cmp_func(lo, pivot);
            if(cmp > 0) {

                while(lo <= hi) {
                    cmp = ctrl->cmp_func(hi, pivot);
                    if(cmp < 0) {
                        QSORT_SWAP(lo, hi);
                        hi -= var_size;
                        break;
                    } else if(cmp == 0) {
                        QSORT_SWAP(hi, pivot_hsect);
                        pivot_hsect -= var_size;
                    }
                    hi -= var_size;
                } /* possible need to break if (hi < start) */
            } else if(cmp == 0) {
                QSORT_SWAP(lo, pivot_lsect);
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
            QSORT_SWAP(lo, pivot_lsect);
            lo -= var_size;
        }
        while(pivot_hsect < end) {
            pivot_hsect += var_size;
            QSORT_SWAP(hi, pivot_hsect);
            hi += var_size;
        }

        /* compute sizes */
        if(lo < start)
            lsize = 0;
        else
            lsize = (lo - start) / var_size + 1;
        if(hi > end)
            hsize = 0;
        else
            hsize = (end - hi) / var_size + 1;
        /* add left and right sections onto stack */
        if(lsize > hsize) {
            if(lsize > BL_SORT_QSORT_THRESHOLD)
                QSORT_PUSH(start, lo);
            if(hsize > BL_SORT_QSORT_THRESHOLD)
                QSORT_PUSH(hi, end);
        } else {
             if(hsize > BL_SORT_QSORT_THRESHOLD)
                QSORT_PUSH(hi, end);
            if(lsize > BL_SORT_QSORT_THRESHOLD)
                QSORT_PUSH(start, lo);
        }

    /* loop back */
    } while(qsort_stack_index != 0);
}

void bl_sort_qsort(void *base, size_t size, size_t var_size, int (*cmp_func)(const void *, const void *))
{
    struct bl_sort_ctrl_t ctrl;
    
    if(size > 1) {
        ctrl.base = base;
        ctrl.size = size;
        ctrl.var_size = var_size;
        ctrl.cmp_func = cmp_func;
        ctrl.ext_vbuf = malloc(var_size);
        bl_sort_qsort_(&ctrl);
        if(BL_SORT_QSORT_THRESHOLD > 1)
           bl_sort_insertion_extbuf_short(&ctrl, size, 0);
        free(ctrl.ext_vbuf);
    }
}

int bl_sort_radix_maxnum(int *base, size_t size)
{
    unsigned i = 0;
    int maxnum = base[0];
    
    for(i = 1; i < size; i++){
        if(base[i] > maxnum) {
            maxnum = base[i];
        }
    }
    
    return maxnum;
}

void bl_sort_radix(int *base, size_t size)
{
    unsigned i = 0;
    int j = 0;
    int maxnum = 0;
    int bucket[10] = {0};
    int *tmp = NULL;
    int sig_dig = 1;
    
    assert(base);
    
    maxnum = bl_sort_radix_maxnum(base, size);
    tmp = malloc(size * sizeof(int));

    while(maxnum / sig_dig > 0) {
        for(i = 0; i < 10; i++) /* zero out bucket */
            bucket[i] = 0;
        
        for(i = 0; i < size; i++) /* number of digits that will go in each bucket */
            bucket[(base[i] / sig_dig) % 10]++;
        
        for(i = 1; i < 10; i++) /* finds index locations for each bucket (digit) */
            bucket[i] += bucket[i - 1];
        
        for(j = size - 1; j >= 0; j--) /* put each number at the correct index for the current digit, keeps order */
            tmp[--bucket[(base[j] / sig_dig) % 10]] = base[j]; /* goes from the top of each bucket index down */
        
        for(i = 0; i < size; i++) /* put back into original array */
            base[i] = tmp[i];
        
        sig_dig *= 10;
    }
    free(tmp);
}
