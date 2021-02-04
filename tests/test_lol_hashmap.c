#include "../hashmap.h"

bool test_hashmap_creation_deletion(void);
bool test_hashmap_insertion(void);
bool test_hashmap_search(void);
bool test_hashmap_deletion(void);
bool test_many_insertions(void);
bool test_single_search(void);
bool test_multisearch(void);

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
    free_word_hashmap(hm);
    return true;
}

bool test_single_search(void) {
    word_hashmap * hm = new_word_hashmap();
    char * key = "key";
    int32_t value = 42;
    word_hashmap_insert(key, value, hm);
    assert(word_hashmap_search(key, hm) == value);
    free_word_hashmap(hm);
    return true;
}

bool test_multisearch(void) {
    word_hashmap * hm = new_word_hashmap();
    char * keys[] = {"key1", "key2", "key3", "key4", "key5"};
    int32_t values[] = {1, 2, 3, 4, 5};
    for (int i=0; i<5; i++)
        word_hashmap_insert(keys[i], values[i], hm);
    for (int i=0; i<5; i++)
        assert(word_hashmap_search(keys[i], hm) == values[i]);
    free_word_hashmap(hm);
    return true;
}

bool test_delete_element(void) {
    word_hashmap * hm = new_word_hashmap();
    char * key = "key";
    int32_t val = 4;
    word_hashmap_insert(key, val, hm);
    word_hashmap_delete(key, hm);
    return word_hashmap_search(key, hm) == 0;
}

bool test_deletion_doesnt_break_chain(void) {
    word_hashmap * hm = new_word_hashmap();
    char * key1 = "!";          // ! => 33 = 1 mod 16
    char * key_collision = "1"; // 1 => 49 = 1 mod 16
    word_hashmap_insert(key1, 42, hm);
    word_hashmap_insert(key_collision, 420, hm);
    word_hashmap_delete(key1, hm);
    return word_hashmap_search(key_collision, hm) == 420;
}

int main(int argc, char ** argv) {
    assert(test_hashmap_creation_deletion() && "creation fails !");
    assert(test_hashmap_insertion() && "insertion fails !");
    assert(test_single_search() && "single search fails !");
    assert(test_many_insertions() && "many insertions fails !");
    assert(test_multisearch() && "multisearch fails !");
    assert(test_deletion_doesnt_break_chain()
           && "deletion breaks chain causing search failure !");
    printf("----- OK : ALL %s TEST PASS ! -----\n", argv[0]);
}
