/*
 * parse.h
 */

#ifndef PARSE_H
#define PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libfapolicyd-analyzer/util/field_array.h>
#include <stdbool.h>

typedef struct parse_context_struct parse_context_t;

/**
 * @brief Allocates and initializes a parse_context_t
 *
 * @return parse_context_t* pointer to a new parse_context_t or NULL if
 * allocation failed
 */
parse_context_t *construct_parse_context(void);

/**
 * @brief Destroys a parse_context_t
 *
 * @param parse_ctx a constructed parse_context pointer
 */
void destroy_parse_context(parse_context_t *parse_ctx_ptr);

/**
 * @brief Imbues the parser with the fields, order of fields and number of
 * fields to parse and allocates internal data structures for parsing
 *
 * @param parse_ctx a constructed parse_context pointer
 * @param line null-terminated line containing an exemplar record to imbue into
 * the parser context
 * @return int 0 = success, -1 = memory allocation faillure, -2 = no fields
 * found in line
 */
int imbue_parser(parse_context_t *parse_ctx_ptr, const char *line);

/**
 * @brief A shallow and destructive parser, but very efficient
 *
 * @param parse_ctx_ptr an imbued parse_context pointer
 * @param line null-terminated line containing a record
 * @return int 0 = success, -1 = fields are not in the order expected, -2
 * expected number of fields did not match the number of fields parsed
 */
int parse(parse_context_t *parse_ctx_ptr, char *line);

/**
 * @brief Get the number of subject fields from the parsed record
 *
 * @param parse_ctx_ptr context of a parsed record
 * @return int the number of subject fields in the parsed record
 */
size_t get_number_of_subject_fields(const parse_context_t *parse_ctx_ptr);

/**
 * @brief Get the number of object fields from the parsed record
 *
 * @param parse_ctx_ptr context of a parsed record
 * @return int the number of object fields in the parsed record
 */
size_t get_number_of_object_fields(const parse_context_t *parse_ctx_ptr);

/**
 * @brief Get a subject field from the parsed record
 *
 * @param parse_ctx_ptr context of a parsed record
 * @param field_ptr address of an allocated field
 * @param field_number the subject field in the record to get
 * @return int 0 on success, -1 on failure
 */
int get_subject_field(const parse_context_t *parse_ctx_ptr, field_t *field_ptr,
                      size_t field_number);

/**
 * @brief Get an object field from the parsed record
 *
 * @param parse_ctx_ptr context of a parsed record
 * @param field_ptr address of an allocated field
 * @param field_number the object field in the record to get
 * @return int 0 on success, -1 on failure
 */
int get_object_field(const parse_context_t *parse_ctx_ptr, field_t *field_ptr,
                     size_t field_number);

/**
 * @brief return True if the line appears to be an audit decision
 *
 * @param line null-terminated line
 * @return true if the line appears to be an audit decision
 * @return false if the line does not appear to be an audit decision
 */
bool is_audit_record(const char *line);

#ifdef __cplusplus
}
#endif

#endif /* PARSE_H */
