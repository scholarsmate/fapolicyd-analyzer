/*
 * parse.c
 */

#include "parse.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Opaque structure that holds the parse context
 */
struct parse_context_struct {
  field_array_t *subject_fields_ptr;
  field_array_t *object_fields_ptr;
  char *_exemplar;
};

parse_context_t *construct_parse_context(void) {
  parse_context_t *parse_ctx_ptr =
      (parse_context_t *)malloc(sizeof(parse_context_t));
  if (parse_ctx_ptr) {
    parse_ctx_ptr->subject_fields_ptr = field_array_construct();
    if (!parse_ctx_ptr->subject_fields_ptr) {
      free(parse_ctx_ptr);
      return NULL;
    }
    parse_ctx_ptr->object_fields_ptr = field_array_construct();
    if (!parse_ctx_ptr->object_fields_ptr) {
      field_array_destroy(parse_ctx_ptr->subject_fields_ptr);
      free(parse_ctx_ptr);
      return NULL;
    }
    parse_ctx_ptr->_exemplar = NULL;
  }
  return parse_ctx_ptr;
}

void destroy_parse_context(parse_context_t *parse_ctx_ptr) {
  field_array_destroy(parse_ctx_ptr->subject_fields_ptr);
  field_array_destroy(parse_ctx_ptr->object_fields_ptr);
  free(parse_ctx_ptr->_exemplar);
  free(parse_ctx_ptr);
}

bool is_audit_record(const char *line) {
  return (line[0] == 'r' && line[1] == 'u' && line[2] == 'l' &&
          line[3] == 'e' && line[4] == '=');
}

inline bool is_rule(const char *line) {
  if (line[0] == 'a') {
    return (line[1] == 'l' && line[2] == 'l' && line[3] == 'o' &&
            line[4] == 'w');
  } else if (line[0] == 'd') {
    return (line[1] == 'e' && line[2] == 'n' && line[3] == 'y');
  }
  return false;
}

int imbue_parser(parse_context_t *parse_ctx_ptr, const char *line) {
  static const char dec[] = "dec=";
  static const size_t dlen = sizeof(dec) - 1;
  field_array_t *fields_ptr;
  int num_fields = 0;
  char *buffer;
  char *probe;

  if (is_rule(line)) {
    size_t llen = strlen(line);
    size_t slen = llen + dlen;
    buffer = malloc(slen);
    if (buffer) {
      memcpy(buffer, dec, dlen);
      memcpy(buffer + dlen, line, llen);
      buffer[slen - 1] = '\0';
    }
  } else if (is_audit_record(line)) {
    buffer = strdup(line);
  } else {
    return PARSE_ERROR;
  }

  if (!buffer) {
    return ALLOCATION_ERROR;
  }
  probe = buffer;
  while (*probe) {
    if (*probe == '=') {
      ++num_fields;
    } else if (*probe == ':') {
      if (!num_fields) {
        return PARSE_ERROR;
      }
      if (field_array_initialize(parse_ctx_ptr->subject_fields_ptr, num_fields) != OK) {
        return ALLOCATION_ERROR;
      }
      num_fields = 0;
    }
    ++probe;
  }
  if (!num_fields) {
    return PARSE_ERROR;
  }
  if (field_array_initialize(parse_ctx_ptr->object_fields_ptr, num_fields) != OK) {
    return ALLOCATION_ERROR;
  }

  probe = buffer;
  num_fields = 0;
  fields_ptr = parse_ctx_ptr->subject_fields_ptr;
  field_array_set_key(fields_ptr, num_fields++, probe);
  while (*probe) {
    if (*probe == ' ') {
      while (*probe == ' ')
        ++probe;
      if (*probe == ':') {
        ++probe;
        while (*probe == ' ')
          ++probe;
        num_fields = 0;
        fields_ptr = parse_ctx_ptr->object_fields_ptr;
      }
      field_array_set_key(fields_ptr, num_fields++, probe);
      continue;
    }
    if (*probe == '=') {
      *probe = '\0';
    }
    ++probe;
  }
  parse_ctx_ptr->_exemplar = buffer;
  return OK;
}

int parse(parse_context_t *parse_ctx_ptr, char *line) {
  char *probe = line;
  size_t parsed_fields = 0;
  field_array_t *fields_ptr = parse_ctx_ptr->subject_fields_ptr;

  while (*probe) {
    if (*probe == '=') {
      *probe = '\0';
      if (0 != strcmp(field_array_get_key(fields_ptr, parsed_fields), line)) {
        return PARSE_ERROR; // field order does not match imbued exemplar
      }
      field_array_set_value(fields_ptr, parsed_fields++, ++probe); // the beginning of a new value
      continue;
    }
    if (*probe == ' ') {
      *probe = '\0'; // terminate the end of the value
      ++probe;
      while (*probe == ' ')
        ++probe; // skip additional spaces
      // colon switches from subject to object
      if (*probe == ':') {
        ++probe;
        while (*probe == ' ')
          ++probe; // skip additional spaces after the colon
        if (parsed_fields != get_number_of_subject_fields(parse_ctx_ptr)) {
          return PARSE_ERROR;
        }
        parsed_fields = 0;
        fields_ptr = parse_ctx_ptr->object_fields_ptr;
      }
      line = probe; // the beginning of a new field
    }
    ++probe;
  }
  if (*(--probe) == '\n')
    *probe = '\0'; // gobble up trailing newlines
  return get_number_of_object_fields(parse_ctx_ptr) == parsed_fields
             ? OK
             : PARSE_ERROR;
}

size_t get_number_of_subject_fields(const parse_context_t *parse_ctx_ptr) {
  return field_array_get_num_fields(parse_ctx_ptr->subject_fields_ptr);
}

size_t get_number_of_object_fields(const parse_context_t *parse_ctx_ptr) {
  return field_array_get_num_fields(parse_ctx_ptr->object_fields_ptr);
}

int get_subject_field(const parse_context_t *parse_ctx_ptr, field_t *field_ptr,
                      size_t field_number) {
  return field_array_get_field(parse_ctx_ptr->subject_fields_ptr, field_ptr, field_number);
}

int get_object_field(const parse_context_t *parse_ctx_ptr, field_t *field_ptr,
                     size_t field_number) {
  return field_array_get_field(parse_ctx_ptr->object_fields_ptr, field_ptr, field_number);
}
