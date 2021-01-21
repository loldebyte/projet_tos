#include "../hashmap.h"
#include <stdio.h>

bool test_hashmap_creation_deletion();
bool test_hashmap_creation_deletion() {
    word_hashmap * hm = new_word_hashmap();
    free_word_hashmap(hm);
    return true;
}

int main(int argc, char ** argv) {
    assert(test_hashmap_creation_deletion() && "creation fails !");
    printf("----- OK : ALL %s TEST PASS ! -----\n", argv[0]);
}
