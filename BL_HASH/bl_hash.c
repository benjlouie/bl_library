/* BL_HASH functions */
/*///////////////////*/

#include "bl_hash.h"

struct bl_hashtable_t {
    size_t tableSize;
    struct bl_hashtable_elm {
        char *key;
        void *data;
    } **table;
};


// 64-bit Pearson hash (From wikipedia)
size_t bl_hash(unsigned char *x, size_t length)
{
    int i, j;
    unsigned char final[8];
    unsigned char T[256] = // 0-255 shuffled randomly
        {45, 21 , 91 , 191, 148, 128, 9  , 49 , 162, 228, 137, 55 , 19 , 136, 112, 210,
        145, 154, 198, 70 , 254, 41 , 51 , 232, 34 , 32 , 156, 180, 37 , 181, 173, 110,
        31 , 39 , 13 , 166, 35 , 122, 50 , 54 , 237, 66 , 116, 81 , 114, 143, 132, 48 ,
        92 , 174, 246, 225, 217, 249, 212, 97 , 131, 69 , 36 , 159, 124, 172, 67 , 215,
        102, 206, 123, 52 , 80 , 0  , 171, 100, 207, 95 , 170, 117, 61 , 248, 141, 65 ,
        158, 160, 14 , 22 , 127, 106, 133, 151, 146, 33 , 113, 87 , 138, 30 , 121, 178,
        142, 7  , 197, 247, 134, 195, 62 , 78 , 234, 43 , 17 , 3  , 227, 169, 233, 140,
        82 , 177, 167, 104, 8  , 139, 241, 168, 94 , 163, 129, 187, 107, 47 , 25 , 221,
        244, 231, 63 , 77 , 190, 235, 209, 199, 179, 71 , 59 , 216, 40 , 2  , 189, 242,
        185, 201, 222, 213, 15 , 224, 20 , 120, 4  , 24 , 76 , 27 , 192, 29 , 23 , 58 ,
        56 , 46 , 108, 196, 18 , 219, 182, 64 , 186, 125, 6  , 93 , 214, 16 , 149, 28 ,
        88 , 152, 144, 111, 150, 245, 238, 90 , 83 , 226, 205, 188, 255, 26 , 57 , 5  ,
        236, 79 , 211, 96 , 68 , 74 , 240, 38 , 12 , 86 , 176, 109, 115, 126, 130, 250,
        200, 44 , 85 , 103, 194, 184, 183, 60 , 84 , 230, 1  , 161, 239, 99 , 75 , 218,
        220, 223, 243, 119, 89 , 253, 135, 193, 73 , 153, 208, 229, 155, 204, 101, 105,
        175, 10 , 118, 251, 11 , 202, 252, 147, 165, 164, 98 , 53 , 72 , 42 , 157, 203};
    
    for(j = 0; j < 8; j++) {
        unsigned char h = T[(x[0] + j) % 256];
        for(i = 1; i < length; i++) {
            h = T[h ^ x[i]];
        }
        final[j] = h;
    }
    // assumes size_t is 64-bit
    return *(size_t *)final;
}

bl_hashtable *bl_hashtable_new(size_t tableSize)
{
    bl_hashtable *ht = malloc(sizeof(bl_hashtable));
    ht->tableSize = tableSize;
    ht->table = calloc(tableSize, sizeof(struct bl_hashtable_elm *));
}

void bl_hashtable_insert(bl_hashtable *ht, char *key, void *data)
{
    // malloc mem for elm
    struct bl_hashtable_elm *elm = malloc(sizeof(struct bl_hashtable_elm));
    // use strdup to copy key string
    
    // conpute hash index from key
    
    // insert elm in table
    
}
