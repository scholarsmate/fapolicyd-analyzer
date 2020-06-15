#include "fields.h"
#include "error_codes.h"
#include <stdlib.h>

struct field_array_stuct {
  int num_fields;
  char **keys;
  char **values;
};

int initialize_field_array(field_array_t *field_array_ptr, size_t num_fields) {
  field_array_ptr->num_fields = 0;
  field_array_ptr->keys = calloc(num_fields + 1, sizeof(char *));
  if (!field_array_ptr->keys) {
    return ALLOCATION_ERROR;
  }
  field_array_ptr->values = calloc(num_fields + 1, sizeof(char *));
  if (!field_array_ptr->values) {
    free(field_array_ptr->keys);
    return ALLOCATION_ERROR;
  }
  field_array_ptr->num_fields = num_fields;
  return OK;
}

field_array_t *construct_field_array(void) {
  field_array_t *field_array_ptr =
      (field_array_t *)malloc(sizeof(field_array_t));
  if (field_array_ptr) {
    field_array_ptr->num_fields = 0;
    field_array_ptr->keys = NULL;
    field_array_ptr->values = NULL;
  }
  return field_array_ptr;
}

void destroy_field_array(field_array_t *field_array_ptr) {
  if (field_array_ptr->keys) {
    free(field_array_ptr->keys);
  }
  if (field_array_ptr->values) {
    free(field_array_ptr->values);
  }
  free(field_array_ptr);
}

void set_num_fields(field_array_t *field_array_ptr, size_t num_fields) {
  field_array_ptr->num_fields = num_fields;
}

void set_key(field_array_t *field_array_ptr, size_t field_number, char *key) {
  field_array_ptr->keys[field_number] = key;
}

void set_value(field_array_t *field_array_ptr, size_t field_number,
               char *value) {
  field_array_ptr->values[field_number] = value;
}

size_t get_num_fields(field_array_t *field_array_ptr) {
  return field_array_ptr->num_fields;
}

char *get_key(field_array_t *field_array_ptr, size_t field_number) {
  return field_array_ptr->keys[field_number];
}

char *get_value(field_array_t *field_array_ptr, size_t field_number) {
  return field_array_ptr->values[field_number];
}

int get_field(field_array_t *field_array_ptr, field_t *field_ptr,
              size_t field_number) {
  if (field_number < get_num_fields(field_array_ptr)) {
    field_ptr->key = get_key(field_array_ptr, field_number);
    field_ptr->value = get_value(field_array_ptr, field_number);
    return OK;
  }
  return RANGE_ERROR;
}
