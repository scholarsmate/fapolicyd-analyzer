#include "hash_table.h"
#include "error_codes.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
 * Private
 ****************************************************************************/
#define HASH_TABLE_DEFAULT_NUM_NODES 1024
#define HASH_TABLE_LOAD_FACTOR 0.65

typedef struct hash_table_node_struct {
  char *key;
  void *value;
  uint64_t hash;
} hash_table_node_t;

typedef struct hash_table_struct {
  hash_table_node_t **nodes;
  size_t number_nodes;
  size_t used_nodes;
} hash_table_t;

static int _assign_node(hash_table_t *table, const char *key, void *value,
                        uint64_t hash, size_t index) {
  hash_table_node_t *new_node_ptr = malloc(sizeof(hash_table_node_t));
  if (new_node_ptr) {
    char *new_key = strdup(key);
    if (new_key) {
      table->nodes[index] = new_node_ptr;
      table->nodes[index]->key = new_key;
      table->nodes[index]->value = value;
      table->nodes[index]->hash = hash;
      return OK;
    }
    free(new_node_ptr);
  }
  return ALLOCATION_ERROR;
}

static void _free_index(hash_table_t *table, size_t index) {
  free(table->nodes[index]->key);
  free(table->nodes[index]);
  table->nodes[index] = NULL;
}

static bool _get_index(const hash_table_t *table, const char *key,
                       uint64_t hash, size_t *index) {
  size_t idx = hash % table->number_nodes;
  size_t i = idx;
  while (1) {
    if (!table->nodes[i]) {
      *index = i;
      return false; // not here OR first open slot
    }
    if (hash == table->nodes[i]->hash &&
        0 == strcmp(key, table->nodes[i]->key)) {
      *index = i;
      return true;
    }
    ++i;
    if (i == table->number_nodes) {
      i = 0;
    }
    if (i == idx) {
      // this means we went all the way around and the table is full
      // this is a logic error that should never happen
      abort();
    }
  }
}

static uint64_t _hash(const char *key) {
  // FNV-1a hash (http://www.isthe.com/chongo/tech/comp/fnv/)
  size_t i;
  const size_t len = strlen(key);
  uint64_t h = 14695981039346656073ULL; // FNV_OFFTABLE 64-bit
  for (i = 0; i < len; ++i) {
    h = h ^ (unsigned char)key[i];
    h = h * 1099511628211ULL; // FNV_PRIME 64-bit
  }
  return h;
}

static int _rehash_table(hash_table_t *table) {
  size_t index;
  size_t i;
  int rc;
  for (i = 0; i < table->number_nodes; ++i) {
    if (table->nodes[i]) {
      if (!_get_index(table, table->nodes[i]->key, table->nodes[i]->hash,
                      &index)) {
        // nodes already in the hash table must have indexes
        // this is a logic error that should never happen
        abort();
      }
      if (i != index) { // we are moving this node
        if (OK != (rc = _assign_node(table, table->nodes[i]->key,
                                     table->nodes[i]->value,
                                     table->nodes[i]->hash, index))) {
          return rc;
        }
        _free_index(table, i);
      }
    }
  }
  return OK;
}

static bool _table_remove(hash_table_t *table, const char *key, uint64_t hash) {
  size_t index;
  if (!_get_index(table, key, hash, &index)) {
    return false;
  }
  // remove this node
  _free_index(table, index);
  --table->used_nodes;
  return true;
}

static int _table_add(hash_table_t *table, const char *key, void *value,
                      uint64_t hash) {
  size_t index;
  // Remove any existing entry
  if (_get_index(table, key, hash, &index)) {
    // remove this node
    _free_index(table, index);
    --table->used_nodes;
  }
  // Expand nodes if we are close to our desired fullness
  if ((float)table->used_nodes / table->number_nodes > HASH_TABLE_LOAD_FACTOR) {
    size_t i;
    size_t orig_num_nodes = table->number_nodes;
    size_t num_nodes = orig_num_nodes << 1; // we want to double each time
    hash_table_node_t **tmp =
        realloc(table->nodes, num_nodes * sizeof(hash_table_node_t *));
    if (tmp == NULL || table->nodes == NULL) {
      return ALLOCATION_ERROR;
    }
    table->nodes = tmp;
    for (i = orig_num_nodes; i < num_nodes; ++i) {
      table->nodes[i] = NULL;
    }
    table->number_nodes = num_nodes;
    // rehash all nodes
    _rehash_table(table);
  }
  _assign_node(table, key, value, hash, index);
  ++table->used_nodes;
  return OK;
}

/*****************************************************************************
 * Public
 ****************************************************************************/

hash_table_t *hash_table_construct(void) {
  hash_table_t *hash_table_ptr = malloc(sizeof(hash_table_t));
  return hash_table_ptr;
}

int hash_table_initialize(hash_table_t *table, size_t num_nodes) {
  num_nodes = (num_nodes > 0) ? num_nodes : HASH_TABLE_DEFAULT_NUM_NODES;
  table->nodes = calloc(num_nodes, sizeof(hash_table_node_t *));
  if (table->nodes == NULL) {
    return ALLOCATION_ERROR;
  }
  table->number_nodes = num_nodes;
  table->used_nodes = 0;
  return OK;
}

void hash_table_clear(hash_table_t *table) {
  size_t i;
  for (i = 0; i < table->number_nodes; ++i) {
    if (table->nodes[i] != NULL) {
      _free_index(table, i);
    }
  }
  table->used_nodes = 0;
}

void hash_table_destroy(hash_table_t *table) {
  hash_table_clear(table);
  free(table->nodes);
  free(table);
}

int hash_table_add(hash_table_t *table, const char *key, void *value) {
  return _table_add(table, key, value, _hash(key));
}

bool hash_table_contains(const hash_table_t *table, const char *key) {
  size_t index;
  return _get_index(table, key, _hash(key), &index);
}

bool hash_table_remove(hash_table_t *table, const char *key) {
  return _table_remove(table, key, _hash(key));
}

size_t hash_table_count(const hash_table_t *table) { return table->used_nodes; }

char const *hash_table_next(const hash_table_t *table, size_t *index,
                            void **value) {
  size_t i;
  for (i = *index; i < table->number_nodes; ++i) {
    if (table->nodes[i] != NULL) {
      *index = i + 1;
      *value = table->nodes[i]->value;
      return table->nodes[i]->key;
    }
  }
  *index = i;
  *value = NULL;
  return NULL;
}
