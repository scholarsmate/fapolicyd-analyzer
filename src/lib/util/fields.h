/*
 * fields.h
 */

#ifndef FIELDS_H
#define FIELDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct field_array_stuct field_array_t;

int initialize_field_array(field_array_t *field_array_ptr, size_t num_fields);

field_array_t *construct_field_array(void);

void destroy_field_array(field_array_t *field_array_ptr);

void set_num_fields(field_array_t *field_array_ptr, size_t num_fields);

void set_key(field_array_t *field_array_ptr, size_t field_number, char *key);

void set_value(field_array_t *field_array_ptr, size_t field_number,
               char *value);

size_t get_num_fields(field_array_t *field_array_ptr);

char *get_key(field_array_t *field_array_ptr, size_t field_number);

char *get_value(field_array_t *field_array_ptr, size_t field_number);

/**
 * @brief A key / value pair of null-terminated strings
 */
typedef struct field_struct {
  const char *key;
  const char *value;
} field_t;

int get_field(field_array_t *field_array_ptr, field_t *field_ptr,
              size_t field_number);

#ifdef __cplusplus
}
#endif

#endif /* FIELDS_H */
