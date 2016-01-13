#include <stdio.h>
#include "bl_hash.h"


int main(void)
{
    int arr_size = 10;
    int arr[arr_size];
    char *strings[10] = {"str01", "str02", "str03", "str04", "str05", "str06", "str07", "str08", "str09", "str10"};
    for(int i = 0; i < arr_size; i++) {
        arr[i] = i;
    }
    
    bl_hashtable *ht = bl_hashtable_new(10);
    
    for(int i = 0; i < arr_size; i++) {
        bl_hashtable_put(ht, strings[i], &arr[i]);
    }
    printTable(ht);
    printf("count = %lu\n", bl_hashtable_count(ht));
    /*
    bl_hashtable_remove(ht, strings[0]);
    bl_hashtable_remove(ht, strings[6]);
    bl_hashtable_remove(ht, strings[9]);
    bl_hashtable_remove(ht, strings[5]);
    */
    
    int newInt = 60;
    int newInt2 = 78;
    // return ptr to old data
    printf("changed data = %d\n", *(int *)bl_hashtable_modify(ht, strings[6], &newInt));
    // had to create new elm, returned ptr to new data
    printf("modified 'new' data = %d\n", *(int *)bl_hashtable_modify(ht, "newString", &newInt2));
    
    printf("\n");
    printTable(ht);
    printf("count = %lu\n", bl_hashtable_count(ht));
    
    return 0;
}
