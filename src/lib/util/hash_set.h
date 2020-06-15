/*
 * hash_set.h
 */

#ifndef HASH_SET_H
#define HASH_SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct hash_set_struct hash_set_t;

hash_set_t *hash_set_construct(void);
int hash_set_initialize(hash_set_t *hash_set_ptr, size_t num_nodes);
void hash_set_destroy(hash_set_t *hash_set_ptr);

void hash_set_clear(hash_set_t *hash_set_ptr);
int hash_set_add(hash_set_t *hash_set_ptr, const char *key);
bool hash_set_contains(const hash_set_t *hash_set_ptr, const char *key);
bool hash_set_remove(hash_set_t *set, const char *key);
size_t hash_set_count(const hash_set_t *hash_set_ptr);
char const *hash_set_next(const hash_set_t *set, size_t *index);

#ifdef __cplusplus
}
#endif

#endif /* HASH_SET_H */