/*
 * hash_table.h
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct hash_table_struct hash_table_t;

hash_table_t * hash_table_construct(void);
int hash_table_initialize(hash_table_t * hash_table_ptr, size_t num_nodes);
void hash_table_destroy(hash_table_t * hash_table_ptr);

void hash_table_clear(hash_table_t * hash_table_ptr);
int hash_table_add(hash_table_t * hash_table_ptr, const char * key, void * value);
bool hash_table_contains(const hash_table_t * hash_table_ptr, const char * key);
bool hash_table_remove(hash_table_t * table, const char * key);
void * hash_table_get(const hash_table_t * hash_table_ptr, const char * key);
size_t hash_table_count(const hash_table_t * hash_table_ptr);
char const * hash_table_next(const hash_table_t * table, size_t * index, void ** value);

#endif /* HASH_TABLE_H */