/*
 * parse.h
 */

#ifndef PARSE_H
#define PARSE_H

typedef struct parse_context_struct parse_context_t;

/**
 * @brief A key / value pair of null-terminated strings
 */
typedef struct field_struct {
    const char * key;
    const char * value;
} field_t;

/**
 * @brief Allocates and initializes a parse_context_t
 * 
 * @return parse_context_t* pointer to a new parse_context_t or NULL if allocation failed
 */
parse_context_t * construct_parse_context(void);

/**
 * @brief Destroys a parse_context_t
 * 
 * @param parse_ctx a constructed parse_context pointer
 */
void destroy_parse_context(parse_context_t * parse_ctx_ptr);

/**
 * @brief Embues the parser with the fields, order of fields and number of fields to parse and allocates internal data structures for parsing
 * 
 * @param parse_ctx a constructed parse_context pointer
 * @param line null-terminated line containing an exemplar record to embue into the parser context
 * @return int 0 = success, -1 = memory allocation faillure, -2 = no fields found in line
 */
int embue_parser(parse_context_t * parse_ctx_ptr, const char * line);

/**
 * @brief A shallow and destructive parser, but very efficient
 * 
 * @param parse_ctx_ptr an embued parse_context pointer
 * @param line null-terminated line containing a record
 * @return int 0 = success, -1 = fields are not in the order expected, -2 expected number of fields did not match the number of fields parsed
 */
int parse(parse_context_t * parse_ctx_ptr, char * line);

/**
 * @brief Get the number of fields from the parsed record
 * 
 * @param parse_ctx_ptr context of a parsed record
 * @return int the number of fields in the parsed record
 */
int get_number_of_fields(const parse_context_t * parse_ctx_ptr);

/**
 * @brief Get a field from the parsed record
 * 
 * @param parse_ctx_ptr context of a parsed record
 * @param field_ptr address of an allocated field
 * @param field_number the field in the record to get
 * @return int 0 on success, -1 on failure
 */
int get_field(const parse_context_t * parse_ctx_ptr, field_t * field_ptr, int field_number);

#endif // PARSE_H
