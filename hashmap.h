#include <stdbool.h>
#include <stdint.h>

// TODO: implement open-adressing hashmap for small ints (int8_t) exclusively
// diffusion of the hashing should be garanteed via modular hashing
// this means the # of buckets should ALWAYS be a power of 2 (maybe even 8 for conveniance ?) !
// modular hashing : h(k) = k % m ; m being the # of buckets
// because we design m to be congruent to 2, this means taking the p lowest bits such that :
// m = 2^p
typedef struct _word_bucket {
    char * key;
    bool in_use;
    int32_t data;
} word_bucket;

typedef struct _word_hashmap {
    int64_t table_size;
    int64_t max_size;
    bucket * bucket_array;
} word_hashmap;

uint32_t get_hash(uint32_t);

// TODO: implement open-adressing hashmap for ints/strings that fit on 64bits (int8_t & 7 chars or less strings)
// TODO: implement a chaining hasmap for long (>7 chars) strings or larger objects
