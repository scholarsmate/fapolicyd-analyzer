/*
 * field_array.h
 */

#ifndef FIELD_ARRAY_H
#define FIELD_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct field_array_stuct field_array_t;

int field_array_initialize(field_array_t *field_array_ptr, size_t num_fields);

field_array_t *field_array_construct(void);

void field_array_destroy(field_array_t *field_array_ptr);

void field_array_set_key(field_array_t *field_array_ptr, size_t field_number, const char *key);

void field_array_set_value(field_array_t *field_array_ptr, size_t field_number, const char *value);

size_t field_array_get_num_fields(const field_array_t *field_array_ptr);

const char *field_array_get_key(const field_array_t *field_array_ptr, size_t field_number);

const char *field_array_get_value(const field_array_t *field_array_ptr, size_t field_number);

/**
 * @brief A key / value pair of null-terminated strings
 */
typedef struct field_struct {
  const char *key;
  const char *value;
} field_t;

int field_array_get_field(field_array_t *field_array_ptr, field_t *field_ptr, size_t field_number);

#ifdef __cplusplus
}
#endif

#endif /* FIELD_ARRAY_H */
