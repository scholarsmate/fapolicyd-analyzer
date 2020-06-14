/*****************************************************************************
 * TESTING
 ****************************************************************************/

#include <lib/util/hash_set.h>
#include <stdio.h>

void emit_set(const hash_set_t * set, FILE * fp) {
    size_t index = 0;
    const char * key;
    while ((key = hash_set_next(set, &index))) {
        fprintf(fp, "key: %s\n", key);
    }
}

int main(int argc, char** argv) {
    hash_set_t * set = hash_set_construct();

    hash_set_initialize(set, 0);
    hash_set_add(set, "orange");
    hash_set_add(set, "blue");
    hash_set_add(set, "red");
    hash_set_add(set, "green");
    hash_set_add(set, "yellow");

    printf("Set contains %lu nodes\n", hash_set_count(set));
    if (hash_set_contains(set, "yellow")) {
        printf("Set contains 'yellow'!\n");
    } else {
        printf("Set does not contain 'yellow'!\n");
    }

    if (hash_set_contains(set, "purple")) {
        printf("Set contains 'purple'!\n");
    } else {
        printf("Set does not contain 'purple'!\n");
    }

    emit_set(set, stdout);
    printf("---\n");
    hash_set_remove(set, "blue");
    emit_set(set, stdout);

    hash_set_destroy(set);
}