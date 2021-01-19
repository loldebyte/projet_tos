#include <"hashmap.h">

uint32_t get_hash(uint32_t key, word_hashmap * hashmap) {
    uint32_t m = hashmap->table_size;
    return key % m; // TODO: optimise this by taking only lowest log2(m) bits
}


