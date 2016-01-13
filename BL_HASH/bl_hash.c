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

// TODO: add delete function to remove all elms and the hash

uint64_t bl_hash64(unsigned char *x, size_t length);
bl_hashtable *bl_hashtable_new(size_t tableSize);
void bl_hashtable_put(bl_hashtable *ht, char *key, void *data);
void *bl_hashtable_get(bl_hashtable *ht, char *key);
void *bl_hashtable_remove(bl_hashtable *ht, char *key);
size_t bl_hashtable_count(bl_hashtable *ht);
void *bl_hashtable_modify(bl_hashtable *ht, char *key, void *newData);
void elm_free(struct bl_hashtable_elm *elm);


// 64-bit Pearson hash (From wikipedia)
uint64_t bl_hash64(unsigned char *x, size_t length)
{
    int i, j;
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
        unsigned char h = T[(x[0] + j) % 256];
        for(i = 1; i < length; i++) {
            h = T[h ^ x[i]];
        }
        final[j] = h;
    }
    // assumes size_t is 64-bit
    return *(uint64_t *)final;
}

bl_hashtable *bl_hashtable_new(size_t tableSize)
{
    bl_hashtable *ht = malloc(sizeof(bl_hashtable));
    ht->tableSize = tableSize;
    ht->count = 0;
    ht->table = calloc(tableSize, sizeof(struct bl_hashtable_elm *));
}

void bl_hashtable_put(bl_hashtable *ht, char *key, void *data)
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
        elm_free(elm);
        ht->count--;
    }
    return data;
}

size_t bl_hashtable_count(bl_hashtable *ht)
{
    return ht->count;
}

/**
 * returns ptr to old data, if new elm was created returns newData ptr
 * */
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

void elm_free(struct bl_hashtable_elm *elm)
{
    free(elm->key); // should always be something
    free(elm);
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
