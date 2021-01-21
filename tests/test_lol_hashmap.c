#include "../hashmap.h"
#include <stdio.h>

bool test_hashmap_creation_deletion(void);
bool test_hashmap_insertion(void);
bool test_hashmap_search(void);
bool test_hashmap_deletion(void);
bool test_many_insertions(void);

bool test_hashmap_creation_deletion(void) {
    word_hashmap * hm = new_word_hashmap();
    free_word_hashmap(hm);
    return true;
}

bool test_hashmap_insertion(void) {
    word_hashmap * hm = new_word_hashmap();
    const char * key = "key";
    const int32_t value = 1;
    word_hashmap_insert(key, value, hm);
    free_word_hashmap(hm);
    return true;
}

bool test_many_insertions(void) {
    word_hashmap * hm = new_word_hashmap();
    char * keys[] = {"key", "key2", "key3", "key4", "key5", "key6", "key7",
                     "key8", "key9", "key10", "key11", "key12", "key13",
                     "key14", "key15", "key16", "key17", "key18", "key19"};
    for (int32_t i = 0; i<19; i++) {
        int32_t value = i;
        word_hashmap_insert(keys[i], value, hm);
    }
    return true;
}

int main(int argc, char ** argv) {
    assert(test_hashmap_creation_deletion() && "creation fails !");
    assert(test_hashmap_insertion() && "insertion fails !");
    assert(test_many_insertions() && "many insertions fails !");
    printf("----- OK : ALL %s TEST PASS ! -----\n", argv[0]);
}
