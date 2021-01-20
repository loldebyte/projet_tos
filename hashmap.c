#include <"hashmap.h">

#define HASHMAP_HARD_SIZE_LIMIT 8589934592 // 2^33
#define HASHMAP_MIN_SIZE 16
#define HASHMAP_MAXIMUM_LOAD_FACTOR 70
#define HASHMAP_MINIMUM_LOAD_FACTOR 10

static word_bucket DELETED_WORD_BUCKET = {NULL, 0};

static uint32_t _hash(const char * key,
                      const uint32_t prime,
                      const uint32_t number_buckets) {
    uint32_t hash = 0;
    int len = strlen(key);
    for (int i=0; i<len; i++)
        hash += (uint32_t) key[i];
    hash = ((uint32_t) hash) % number_buckets;
    return (uint32_t) hash;
}

static uint32_t get_hash(const char * key,
                         const uint32_t number_buckets,
                         const uint32_t attempt) {
    const uint32_t hash1 = _hash(key, HASH_PRIME_1, number_buckets);
    const uint32_t hash2 = _hash(key, HASH_PRIME_2, number_buckets);
    return (hash1 + attempt*(hash2 + 1)%number_buckets);
}

static word_bucket * _new_word_bucket(const char * key, int32_t value) {
    word_bucket * new_bucket = calloc(sizeof(word_bucket));
    new_bucket->data = value;
    new_bucket->key = strdup(key);
    return new_bucket;
}

static void _free_word_bucket(word_bucket * wb) {
    wb->value = 0;
    wb->key = NULL;
    free(wb->key);
    wb = NULL;
    free(wb);
}

word_hashmap * new_word_hashmap() {
    new_word_hashmap_sized(HASHMAP_MIN_SIZE);
}

word_hashmap * new_word_hashmap_sized(const uint32_t new_size) {
    word_hashmap * hm = malloc(sizeof(word_hashmap));
    assert(!(new_size & 1) && "You must enter a power of 2 as a size.");
    hm->max_size = new_size;
    hm->table_size = 0;
    hm->bucket_array = calloc((size_t) hm->max_size, size_t(word_bucket *));
    return hm;
}

static void _word_hasmap_resize_up(word_hashmap * hm) {
    if (hm->max_size == HASHMAP_HARD_SIZE_LIMIT)
        return;
    word_hashmap * new_hm = new_word_hashmap_sized(hm->max_size);
    new_hm->max_size <<= 1;
    new_hm->table_size = hm->table_size;
    new_hm->bucket_array = calloc((size_t) new_hm->max_size, size_t(word_bucket *));
    assert(new_hm != NULL && "fatal : calloc failed during resizing");

    for (int i=0; i<hm->table_size; i++) {
        word_bucket * bucket = hm->bucket_array[i];
        if (bucket != NULL && bucket != &DELETED_WORD_BUCKET)
            word_hashmap_insert(bucket->key, bucket->value, new_hm);
    }
    
    const int tmp_maxsize = hm->max_size;
    hm->max_size = new_hm->max_size;
    hm->table_size = new_hm->table_size;
    new_hm->max_size = tmp_maxsize;

    const word_bucket ** tmp_bucket_array = ht->bucket_array;
    hm->bucket_array = new_hm->bucket_array;
    new_hm->bucket_array = tmp_bucket_array;
    free_word_hashmap(new_hm);
} 

static void _word_hasmap_resize_down(word_hashmap * hm) {
    if (hm->max_size == HASHMAP_MIN_SIZE)
        return;
    word_hashmap * new_hm = new_word_hashmap_sized(hm->max_size);
    new_hm->max_size >>= 1;
    new_hm->table_size = hm->table_size;
    new_hm->bucket_array = calloc((size_t) new_hm->max_size, size_t(word_bucket *));
    assert(new_hm != NULL && "fatal : calloc failed during resizing");

    for (int i=0; i<hm->table_size; i++) {
        word_bucket * bucket = hm->bucket_array[i];
        if (bucket != NULL && bucket != &DELETED_WORD_BUCKET)
            word_hashmap_insert(bucket->key, bucket->value, new_hm);
    }
    
    hm->max_size = new_hm->max_size;
    hm->table_size = new_hm->table_size;

    hm->bucket_array = new_hm->bucket_array;
    free_word_hashmap(new_hm);
}

void free_word_hashmap(word_hashmap * hm) {
    for (int i=0; i<hm->table_size; i++) {
        word_bucket * bucket = hm->bucket_array[i];
        if (word_bucket != NULL)
            _free_word_bucket(bucket);
    }
    hm->table_size = 0;
    hm->max_size = 0;
    hm->bucket_array = NULL;
    free(hm->bucket_array);
    hm = NULL;
    free(hm);
}

bool word_hashmap_insert(const char * key, uint32_t value, word_hashmap * hm) {
    const uint32_t load = (hm->table_size+1)*100 / hm->max_size;
    if (load > 70)
        _word_hasmap_resize_up(hm);
    word_bucket * new_bucket = _new_word_bucket(key, value);
    uint32_t index = get_hash(key, hm->max_size, 0);

    word_bucket * current_bucket;
    int i = 1;
    while (current_bucket = hm->bucket_array[index];
           current_bucket != NULL && current_bucket != &DELETED_WORD_BUCKET) {
        if (strcmp(current_bucket->key, key) == 0) {
            _free_word_bucket(current_bucket);
            hm->bucket_array[index] = new_bucket;
            return true;
        }
        index = get_hash(key, hm->max_size, i);
        i++;
    }
    
    ht->bucket_array[index] = new_bucket;
    ht->table_size++;
    return true;
}

int32_t word_hashmap_search(const char * key, word_hashmap * hm) {
    uint32_t index = get_hash(key, hm->max_size, 0);
    word_bucket * current_bucket;
    int i = 1;
    while (current_bucket = hm->bucket_array[index];
           current_bucket != NULL && current_bucket != &DELETED_WORD_BUCKET) {
        if (strcmp(current_bucket->key, key) == 0)
            return current_bucket->value;
        index = get_hash(key, hm->max_size, i);
        i++;
    }
    return (int32_t) NULL;
}

void word_hashmap_delete(const char * key, word_hashmap * hm) {
    if (hm->table_size > 0) {
        const uint32_t load = hm->table_size*100 / hm->max_size;
        if (load < 10)
            _word_hasmap_resize_down(hm);
    }

    uint32_t index = get_hash(key, hm->max_size, 0);
    word_bucket * current_bucket;
    int i = 1;
    while (current_bucket = hm->bucket_array[index]; current_bucket != NULL) {
        if (current_bucket != &DELETED_WORD_BUCKET) {
            if (strcmp(current_bucket->key, key) == 0) {
                _free_word_bucket(current_bucket);
                hm->bucket_array[index] = &DELETED_WORD_BUCKET;
                hm->table_size--;
            }
        }
        index = get_hash(key, hm->max_size, i);
        i++;
    }
}


