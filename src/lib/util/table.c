#include "table.h"
#include "detail/macros.h"
#include "detail/utarray.h"
#include "detail/uthash.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
 * Table
 ****************************************************************************/

struct table_struct {
  size_t column_count;
  char **column_names;
  UT_array **columns;
};

table_t *table_construct(void) {
  table_t *table_ptr = NULL;
  CHECK_PTR(table_ptr = malloc(sizeof(table_t)));
  table_ptr->column_count = 0;
  table_ptr->column_names = NULL;
  table_ptr->columns = NULL;
  return table_ptr;
}

void table_destroy(table_t *table_ptr) {
  if (table_ptr) {
    size_t i = 0;
    const size_t column_count = table_ptr->column_count;
    for (i = 0; i < column_count; ++i) {
      free(table_ptr->column_names[i]);
      utarray_free(table_ptr->columns[i]);
    }
    free(table_ptr->column_names);
    free(table_ptr->columns);
    free(table_ptr);
  }
}

int table_initialize(table_t *table_ptr, const char **column_names, size_t column_count) {
  char *col_name = NULL;
  char **col_names = NULL;
  UT_array **cols = NULL;
  size_t i = 0;

  CHECK_PTR(col_names = calloc(sizeof(const char *), column_count));
  CHECK_PTR(cols = calloc(sizeof(UT_array), column_count));

  /* store the column names */
  for (i = 0; i < column_count; ++i) {
    CHECK_PTR(col_name = strdup(column_names[i]));
    col_names[i] = col_name;
  }

  for (i = 0; i < column_count; ++i) {
    utarray_new(cols[i], &ut_str_icd); /* array created for c strings */
  }

  table_ptr->column_count = column_count;
  table_ptr->column_names = col_names;
  table_ptr->columns = cols;
  return OK;
}

size_t table_get_column_count(const table_t *table_ptr) { return table_ptr->column_count; }

size_t table_get_row_count(const table_t *table_ptr) { return utarray_len(table_ptr->columns[0]); }

const char *table_get_column_name(const table_t *table_ptr, size_t col_num) { return table_ptr->column_names[col_num]; }

int table_find_column_number(const table_t *table_ptr, const char *col_name, size_t *col_num_ptr) {
  const size_t column_count = table_get_column_count(table_ptr);
  size_t i = 0;

  for (i = 0; i < column_count; ++i) {
    if (0 == strcmp(col_name, table_get_column_name(table_ptr, i))) {
      *col_num_ptr = i;
      return OK;
    }
  }
  return NOT_FOUND;
}

const char *table_get_value(const table_t *table_ptr, size_t col_num, size_t row_num) {
  return *(const char **)utarray_eltptr(table_ptr->columns[col_num], row_num);
}

int table_append_row(table_t *table_ptr, const char **row) {
  size_t i;
  const size_t column_count = table_get_column_count(table_ptr);
  for (i = 0; i < column_count; ++i) {
    utarray_push_back(table_ptr->columns[i], &row[i]);
  }
  return OK;
}

int table_append_column(table_t *table_ptr, const char *column_name, const char **column) {
  size_t i = 0;
  const size_t row_count = table_get_row_count(table_ptr);
  UT_array *new_col = NULL;

  utarray_new(new_col, &ut_str_icd);
  utarray_reserve(new_col, row_count);
  for (i = 0; i < row_count; ++i) {
    utarray_push_back(new_col, &column[i]);
  }

  const size_t column_count = ++table_ptr->column_count;
  CHECK_PTR(table_ptr->column_names = realloc(table_ptr->column_names, column_count * sizeof(const char *)));
  CHECK_PTR(table_ptr->columns = realloc(table_ptr->columns, column_count * sizeof(UT_array)));
  CHECK_PTR(table_ptr->column_names[column_count - 1] = strdup(column_name));
  table_ptr->columns[column_count - 1] = new_col;

  return OK;
}

/*****************************************************************************
 * Table Index
 ****************************************************************************/

struct row_index_struct {
  UT_array *rows;
};

struct table_index_struct {
  const char *key; /* key */
  row_index_t index;
  UT_hash_handle hh; /* makes this structure hashable */
};

static const UT_icd size_t_icd = {sizeof(size_t), NULL, NULL, NULL};

table_index_t *table_index_create(const table_t *table_ptr, size_t col_num) {
  table_index_t *table_index_ptr = NULL;
  table_index_t *probe = NULL;
  table_index_t *item = NULL;
  const char *key = NULL;
  const size_t row_count = table_get_row_count(table_ptr);
  size_t i = 0;

  for (i = 0; i < row_count; ++i) {
    key = table_get_value(table_ptr, col_num, i);
    HASH_FIND_STR(table_index_ptr, key, probe);
    if (probe) {
      utarray_push_back(probe->index.rows, &i);
    } else {
      CHECK_PTR(item = malloc(sizeof(table_index_t)));
      item->key = key;
      utarray_new(item->index.rows, &size_t_icd);
      utarray_push_back(item->index.rows, &i);
      HASH_ADD_KEYPTR(hh, table_index_ptr, item->key, strlen(item->key), item);
    }
  }
  return table_index_ptr;
}

void table_index_destroy(table_index_t *table_index_ptr) {
  table_index_t *s = NULL;
  table_index_t *tmp = NULL;

  if (table_index_ptr) {
    /* free the hash table contents */
    HASH_ITER(hh, table_index_ptr, s, tmp) {
      HASH_DEL(table_index_ptr, s);
      utarray_free(s->index.rows);
      free(s);
    }
  }
}

size_t table_index_count(const table_index_t *table_index_ptr) { return HASH_COUNT(table_index_ptr); }

const row_index_t *table_index_find(const table_index_t *table_index_ptr, const char *key) {
  table_index_t *probe = NULL;
  HASH_FIND_STR(table_index_ptr, key, probe);
  return (probe) ? &probe->index : NULL;
}

size_t *row_index_next(const row_index_t *row_index_ptr, size_t *index_p) {
  index_p = (size_t *)utarray_next(row_index_ptr->rows, index_p);
  return index_p;
}
