#include <stdio.h>
#include "bl_hash.h"

void plus(char *key, void *data, void *user_data);

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
        bl_hashtable_insert(ht, strings[i], strlen(strings[i]) + 1, &arr[i]);
    }
    printTable(ht);
    printf("count = %lu\n", bl_hashtable_count(ht));
    /*
    bl_hashtable_remove(ht, strings[0], strlen(strings[0]) + 1);
    bl_hashtable_remove(ht, strings[6], strlen(strings[6]) + 1);
    bl_hashtable_remove(ht, strings[9], strlen(strings[9]) + 1);
    bl_hashtable_remove(ht, strings[5], strlen(strings[5]) + 1);
    */
    
    int newInt = 60;
    int newInt2 = 78;
    // return ptr to old data
    printf("changed data = %d\n", *(int *)bl_hashtable_modify(ht, strings[6], strlen(strings[6]) + 1, &newInt));
    // had to create new elm, returned ptr to new data
    printf("'modified' new data = %d\n", *(int *)bl_hashtable_modify(ht, "newString", strlen("newString") + 1, &newInt2));
    
    printf("\n");
    printTable(ht);
    printf("count = %lu\n", bl_hashtable_count(ht));
    
    int user_int = 5;
    bl_hashtable_foreach(ht, &user_int, (plus));
    
    printf("\n");
    printTable(ht);
    printf("count = %lu\n", bl_hashtable_count(ht));
    
    printf("\n");
    bl_hashtable_rehash(ht, 6);
    printTable(ht);
    printf("count = %lu\n", bl_hashtable_count(ht));
    
    bl_hashtable_foreach_remove(ht, NULL, NULL);
    //bl_hashtable_free(ht); // gets rid of the hash entirely
    
    printf("\n");
    printTable(ht);
    printf("count = %lu\n", bl_hashtable_count(ht));
    
    return 0;
}

void plus(char *key, void *data, void *user_data)
{
    (*(int *)data) += *(int *)user_data;
}

