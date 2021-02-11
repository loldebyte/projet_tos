#ifndef LOL_HASHMAP_H
#define LOL_HASHMAP_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_HARD_SIZE_LIMIT 8589934592 // 2^33
#define HASHMAP_MIN_SIZE 16
#define HASHMAP_MAXIMUM_LOAD_FACTOR 70
#define HASHMAP_MINIMUM_LOAD_FACTOR 10
#define HASH_PRIME_1 13
#define HASH_PRIME_2 53

typedef struct _word_bucket {
    char * key;
    int32_t data;
} word_bucket;

typedef struct _word_hashmap {
    uint32_t table_size;
    uint32_t max_size;
    word_bucket ** bucket_array;
} word_hashmap;

uint32_t _hash(const char *, const uint32_t);
word_bucket * _new_word_bucket(const char *, int32_t);
void _free_word_bucket(word_bucket **);
word_hashmap * new_word_hashmap(void);
word_hashmap * new_word_hashmap_sized(const uint32_t);
void _word_hashmap_resize_up(word_hashmap *);
void _word_hashmap_resize_down(word_hashmap *);
void free_word_hashmap(word_hashmap *);

bool word_hashmap_insert(const char *, uint32_t, word_hashmap *);
int32_t word_hashmap_search(const char *, word_hashmap *);
void word_hashmap_delete(const char *, word_hashmap *);

typedef struct _string_bucket {
    char * key;
    char * data;
} string_bucket;

typedef struct _string_hashmap {
    uint32_t table_size;
    uint32_t max_size;
    string_bucket ** bucket_array;
} string_hashmap;

string_hashmap * new_string_hashmap();

#endif

