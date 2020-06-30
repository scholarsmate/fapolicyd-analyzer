#include "array_list.h"
#include "error_codes.h"
#include <stdlib.h>

#define ARRAY_LIST_DEFAULT_CAPACITY 1024

struct array_list_struct {
  size_t size;
  size_t capacity;
  void **values;
};

array_list_t *array_list_construct(void) {
  array_list_t *array_list_ptr = malloc(sizeof(array_list_t));
  if (array_list_ptr) {
    array_list_ptr->size = 0;
    array_list_ptr->capacity = 0;
    array_list_ptr->values = NULL;
  }
  return array_list_ptr;
}

int array_list_initialize(array_list_t *list, size_t size) {
  if (list->capacity < size) {
    size_t new_capacity = (list->capacity < ARRAY_LIST_DEFAULT_CAPACITY) ? ARRAY_LIST_DEFAULT_CAPACITY : list->capacity;
    while (new_capacity < size) {
      new_capacity *= 2;
    }
    list->values = realloc(list->values, sizeof(void *) * new_capacity);
    if (!list->values) {
      return ALLOCATION_ERROR;
    }
    list->capacity = new_capacity;
  }
  return OK;
}

void array_list_destroy(array_list_t *list) {
  free(list->values);
  free(list);
}

size_t array_list_size(const array_list_t *list) { return list->size; }

void *array_list_get(const array_list_t *list, size_t index) {
  if (index < list->size) {
    return list->values[index];
  }
  return NULL;
}

int array_list_set(array_list_t *list, size_t index, void *value) {
  if (index < list->size) {
    list->values[index] = value;
    return OK;
  }
  return RANGE_ERROR;
}

int array_list_push(array_list_t *list, void *value) {
  int rc = array_list_initialize(list, list->size + 1);
  if (rc == OK) {
    list->values[list->size++] = value;
  }
  return rc;
}

void *array_list_pop(array_list_t *list) {
  if (0 < list->size) {
    return list->values[--list->size];
  }
  return NULL;
}

void array_list_clear(array_list_t *list) { list->size = 0; }
