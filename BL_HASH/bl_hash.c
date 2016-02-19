/* BL_HASH functions */
/*///////////////////*/

#include "bl_hash.h"

struct bl_hashtable_t {
    size_t tableSize;
    size_t count;
    struct bl_hashtable_elm {
        char *key;
        void *data;
        struct bl_hashtable_elm *next;
    } **table;
};

uint64_t bl_hash64(char *x, size_t length);
bl_hashtable *bl_hashtable_new(size_t tableSize);
void bl_hashtable_insert(bl_hashtable *ht, char *key, void *data);
void *bl_hashtable_get(bl_hashtable *ht, char *key);
void *bl_hashtable_remove(bl_hashtable *ht, char *key);
size_t bl_hashtable_count(bl_hashtable *ht);
void *bl_hashtable_modify(bl_hashtable *ht, char *key, void *newData);
void bl_hashtable_foreach(bl_hashtable *ht, void *userData, void (*func)(char *key, void *data, void *userData));
void bl_hashtable_foreach_remove(bl_hashtable *ht, void *userData, void (*func)(char *key, void *data, void *userData));
void bl_hashtable_free(bl_hashtable *ht);


// 64-bit Pearson hash (From wikipedia)
/**
 * creates a 64-bit number of the hashed input
 * @param x the data to hash
 * @param length the length of the data in bytes
 * @return 64-bit integer
 */
uint64_t bl_hash64(char *x, size_t length)
{
    size_t i, j;
    unsigned char final[8];
    unsigned char T[256] = // 0-255 shuffled randomly
        {45,  21,  91, 191, 148, 128,   9,  49, 162, 228, 137,  55,  19, 136, 112, 210,
        145, 154, 198,  70, 254,  41,  51, 232,  34,  32, 156, 180,  37, 181, 173, 110,
         31,  39,  13, 166,  35, 122,  50,  54, 237,  66, 116,  81, 114, 143, 132,  48,
         92, 174, 246, 225, 217, 249, 212,  97, 131,  69,  36, 159, 124, 172,  67, 215,
        102, 206, 123,  52,  80,   0, 171, 100, 207,  95, 170, 117,  61, 248, 141,  65,
        158, 160,  14,  22, 127, 106, 133, 151, 146,  33, 113,  87, 138,  30, 121, 178,
        142,   7, 197, 247, 134, 195,  62,  78, 234,  43,  17,   3, 227, 169, 233, 140,
         82, 177, 167, 104,   8, 139, 241, 168,  94, 163, 129, 187, 107,  47,  25, 221,
        244, 231,  63,  77, 190, 235, 209, 199, 179,  71,  59, 216,  40,   2, 189, 242,
        185, 201, 222, 213,  15, 224,  20, 120,   4,  24,  76,  27, 192,  29,  23,  58,
         56,  46, 108, 196,  18, 219, 182,  64, 186, 125,   6,  93, 214,  16, 149,  28,
         88, 152, 144, 111, 150, 245, 238,  90,  83, 226, 205, 188, 255,  26,  57,   5,
        236,  79, 211,  96,  68,  74, 240,  38,  12,  86, 176, 109, 115, 126, 130, 250,
        200,  44,  85, 103, 194, 184, 183,  60,  84, 230,   1, 161, 239,  99,  75, 218,
        220, 223, 243, 119,  89, 253, 135, 193,  73, 153, 208, 229, 155, 204, 101, 105,
        175,  10, 118, 251,  11, 202, 252, 147, 165, 164,  98,  53,  72,  42, 157, 203};
    
    for(j = 0; j < 8; j++) {
        unsigned char h = T[((unsigned char)x[0] + j) % 256];
        for(i = 1; i < length; i++) {
            h = T[h ^ (unsigned char)x[i]];
        }
        final[j] = h;
    }
    // assumes size_t is 64-bit
    return *(uint64_t *)final;
}

/**
 * creates a new hash table (chained hash) of the specified size
 * @param tableSize the size of the hash table
 * @return the new hashtable
 */
bl_hashtable *bl_hashtable_new(size_t tableSize)
{
    bl_hashtable *ht = malloc(sizeof(bl_hashtable));
    ht->tableSize = tableSize;
    ht->count = 0;
    ht->table = calloc(tableSize, sizeof(struct bl_hashtable_elm *));
    return ht;
}

/**
 * insets a new key value pair in to the hash table
 * @param ht the hash table to insert into
 * @param key the key for the new element (must be a null terminated string)
 * @param data the data to insert with the key
 */
void bl_hashtable_insert(bl_hashtable *ht, char *key, void *data)
{
    // malloc mem for elm
    struct bl_hashtable_elm *elm = malloc(sizeof(struct bl_hashtable_elm));
    // use strdup to copy key string
    elm->key = strdup(key);
    elm->data = data;
    
    // conpute hash index from key
    uint64_t index = bl_hash64(elm->key, strlen(elm->key));
    index %= ht->tableSize;
    
    // insert elm in table
    elm->next = ht->table[index];
    ht->table[index] = elm;
    ht->count++;
}

/**
 * gets the data at the specified key value (first found)
 * @param ht the hash table to look in
 * @param key the key value to find (must be a null terminated string)
 * @return the data found with the specified key value, NULL if not found
 */
void *bl_hashtable_get(bl_hashtable *ht, char *key)
{
    uint64_t index = bl_hash64(key, strlen(key));
    index %= ht->tableSize;
    
    struct bl_hashtable_elm *elm = ht->table[index];
    while(elm) { /* locate matching key */
        if(strcmp(elm->key, key) == 0) {
            break;
        }
        elm = elm->next;
    }
    if(elm) {
        return elm->data;
    } else {
        return NULL;
    }
}

/**
 * removes the first key value pair found specified by key
 * @param ht the hash table to remove from
 * @param key the key to look for (must be a null terminated string)
 * @return data ptr from the removed elm, NULL if the elm was not found
 */
void *bl_hashtable_remove(bl_hashtable *ht, char *key)
{
    uint64_t index = bl_hash64(key, strlen(key));
    index %= ht->tableSize;
    
    void *data = NULL;
    struct bl_hashtable_elm *elm = ht->table[index];
    struct bl_hashtable_elm *prev = NULL;
    
    while(elm) { /* locate matching key */
        if(strcmp(elm->key, key) == 0) {
            break;
        }
        prev = elm;
        elm = elm->next;
    }
    if(elm) {
        if(prev) {
            prev->next = elm->next;
        } else {
            ht->table[index] = elm->next;
        }
        data = elm->data;
        free(elm->key); // free elm
        free(elm);
        ht->count--;
    }
    return data;
}

/**
 * gives the number of key value pairs in the table
 * @param ht the hash table
 * @return the number of elms in the table
 */
size_t bl_hashtable_count(bl_hashtable *ht)
{
    return ht->count;
}

/**
 * gives the size of the hash table
 * @param ht the hash table
 * @return the table size
 */
size_t bl_hashtable_tablesize(bl_hashtable *ht)
{
    return ht->tableSize;
}

/**
 * modifies the data of the first found elm with a matching key, adds a new key value pair if not found
 * @param ht the hash table
 * @param key the key to look for (must be a null terminated string)
 * @param newData the new data to replace the old with (or insert)
 * @return ptr to the old data if it was replaced, ptr to newData if a new elm was inserted
 */
void *bl_hashtable_modify(bl_hashtable *ht, char *key, void *newData)
{
    uint64_t index = bl_hash64(key, strlen(key));
    index %= ht->tableSize;
    
    void *data; // old data (or newData if elm is created)
    struct bl_hashtable_elm *elm = ht->table[index];
    while(elm) {
        if(strcmp(elm->key, key) == 0) {
            break;
        }
        elm = elm->next;
    }
    if(elm) { // found elm
        data = elm->data;
        elm->data = newData;
    } else { // no such elm, add new elm to table
        data = newData;
        elm = malloc(sizeof(struct bl_hashtable_elm));
        elm->key = strdup(key);
        elm->data = newData;
        elm->next = ht->table[index];
        ht->table[index] = elm;
        ht->count++;
    }
    return data;
}

/**
 * runs the function on every elm of the hash table, calls func with the specified userData and each element's key and data
 * @param ht the hash table
 * @param userData the data that is sent with each key value pair to func()
 * @param func() function that is called on each elm of the table, sent that elements key and data along with the provided userData
 * @note func() should not free() or modify the key passed to it
 */
void bl_hashtable_foreach(bl_hashtable *ht, void *userData, void (*func)(char *key, void *data, void *userData))
{
    for(int i = 0; i < ht->tableSize; i++) {
        struct bl_hashtable_elm *elm = ht->table[i];
        while(elm) {
            if(func) {
                func(elm->key, elm->data, userData);
            }
            elm = elm->next;
        }
    }
}

/**
 * runs the function on every elm of the hash table and removes every elm, calls func with the specified userData and each element's key and data
 * @param ht the hash table
 * @param userData the data that is sent with each key value pair to func()
 * @param func() function that is called on each elm of the table, sent that elements key and data along with the provided userData
 * @note func() should not free() or modify the key passed to it
 */
void bl_hashtable_foreach_remove(bl_hashtable *ht, void *userData, void (*func)(char *key, void *data, void *userData))
{
    for(size_t i = 0; i < ht->tableSize; i++) {
        struct bl_hashtable_elm *elm = ht->table[i];
        while(elm) {
            struct bl_hashtable_elm *prev = elm;
            if(func) {
                func(elm->key, elm->data, userData);
            }
            elm = elm->next;
            free(prev->key);
            free(prev);
        }
        ht->table[i] = NULL;
    }
    ht->count = 0;
}

/**
 * frees all hash table resources, does not touch data that was inserted
 * @param ht the hash table to free
 */
void bl_hashtable_free(bl_hashtable *ht)
{
    bl_hashtable_foreach_remove(ht, NULL, NULL);
    free(ht->table);
    free(ht);
}

// TODO: delete this function
void printTable(bl_hashtable *ht)
{
    for(int i = 0; i < ht->tableSize; i++) {
        struct bl_hashtable_elm *elm = ht->table[i];
        printf("%03d| ", i);
        while(elm) {
            printf("%s : %d,  ", elm->key, *(int *)elm->data);
            elm = elm->next;
        }
        printf("\n");
    }
}

