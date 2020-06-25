/*
 * array_list.h
 */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct array_list_struct array_list_t;

array_list_t *array_list_construct(void);
int array_list_initialize(array_list_t *list, size_t size);
void array_list_destroy(array_list_t *list);

size_t array_list_size(const array_list_t *list);
void *array_list_get(const array_list_t *list, size_t index);
int array_list_set(array_list_t *list, size_t index, void *value);
int array_list_push(array_list_t *list, void *value);
void *array_list_pop(array_list_t *list);
void array_list_clear(array_list_t *list);

#ifdef __cplusplus
}
#endif

#endif /* ARRAY_LIST_H */
