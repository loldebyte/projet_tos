#ifndef LOL_HASHMAP_H
#define LOL_HASHMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// TODO: implement open-adressing hashmap for small ints (int8_t) exclusively
// diffusion of the hashing should be garanteed via modular hashing
// this means the # of buckets should ALWAYS be a power of 2 (maybe even 8 for conveniance ?) !
// modular hashing : h(k) = k % m ; m being the # of buckets
// because we design m to be congruent to 2, this means taking the p lowest bits such that :
// m = 2^p
typedef struct _word_bucket {
    char * key;
    int32_t data;
} word_bucket;

typedef struct _word_hashmap {
    uint32_t table_size;
    uint32_t max_size;
    word_bucket ** bucket_array;
} word_hashmap;

static uint32_t _hash(const char *, const uint32_t, const uint32_t);
static uint32_t get_hash(const char *, const uint32_t, const uint32_t);
static word_bucket * _new_word_bucket(const char *, uint32_t);
static void _free_word_bucket(word_bucket *);
word_hashmap * new_word_hashmap(void);
word_hashmap * new_word_hashmap_sized(const uint32_t);
static void _word_hashmap_resize_up(word_hashmap *);
static void _word_hasmap_resize_down(word_hashmap *);
void free_word_hashmap(word_hashmap *);

bool word_hashmap_insert(const char *, uint32_t, word_hashmap *);
int32_t word_hashmap_search(const char *, word_hashmap *);
void word_hashmap_delete(const char *, word_hashmap *);

// TODO: implement open-adressing hashmap for ints/strings that fit on 64bits (int8_t & 7 chars or less strings)
// TODO: implement a chaining hasmap for long (>7 chars) strings or larger objects
#endif

