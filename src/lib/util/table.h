#ifndef TABLE_H
#define TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/*****************************************************************************
 * Table
 ****************************************************************************/
typedef struct table_struct table_t;

table_t *table_construct(void);
void table_destroy(table_t *table_ptr);

int table_initialize(table_t *table_ptr, const char **column_names,
                     size_t column_count);

size_t table_get_column_count(const table_t *table_ptr);
size_t table_get_row_count(const table_t *table_ptr);
const char *table_get_column_name(const table_t *table_ptr, size_t col_num);
const char *table_get_value(const table_t *table_ptr, size_t col_num,
                            size_t row_num);

int table_append(table_t *table_ptr, const char **row);

/*****************************************************************************
 * Table Index
 ****************************************************************************/
typedef struct table_index_struct table_index_t;
typedef struct row_index_struct row_index_t;

table_index_t *table_index_create(const table_t *table_ptr, size_t col_num);
void table_index_destroy(table_index_t *table_index_ptr);
size_t table_index_count(const table_index_t *table_index_ptr);
const row_index_t *table_index_find(const table_index_t *table_index_ptr,
                                    const char *key);
size_t *row_index_next(const row_index_t *row_index_ptr, size_t *index_p);

#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */
