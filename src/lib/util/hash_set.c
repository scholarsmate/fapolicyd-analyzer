#include "hash_set.h"
#include "error_codes.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
 * Private
 ****************************************************************************/
#define HASH_SET_DEFAULT_NUM_NODES 1024
#define HASH_SET_LOAD_FACTOR 0.65

typedef struct hash_set_node_struct {
  char *key;
  uint64_t hash;
} hash_set_node_t;

typedef struct hash_set_struct {
  hash_set_node_t **nodes;
  size_t number_nodes;
  size_t used_nodes;
} hash_set_t;

static int _assign_node(hash_set_t *set, const char *key, uint64_t hash,
                        size_t index) {
  hash_set_node_t *new_node_ptr = malloc(sizeof(hash_set_node_t));
  if (new_node_ptr) {
    char *new_key = strdup(key);
    if (new_key) {
      set->nodes[index] = new_node_ptr;
      set->nodes[index]->key = new_key;
      set->nodes[index]->hash = hash;
      return OK;
    }
    free(new_node_ptr);
  }
  return ALLOCATION_ERROR;
}

static void _free_index(hash_set_t *set, size_t index) {
  free(set->nodes[index]->key);
  free(set->nodes[index]);
  set->nodes[index] = NULL;
}

static bool _get_index(const hash_set_t *set, const char *key, uint64_t hash,
                       size_t *index) {
  size_t idx = hash % set->number_nodes;
  size_t i = idx;
  while (1) {
    if (!set->nodes[i]) {
      *index = i;
      return false; // not here OR first open slot
    }
    if (hash == set->nodes[i]->hash && 0 == strcmp(key, set->nodes[i]->key)) {
      *index = i;
      return true;
    }
    ++i;
    if (i == set->number_nodes) {
      i = 0;
    }
    if (i == idx) {
      // this means we went all the way around and the set is full
      // this is a logic error that should never happen
      abort();
    }
  }
}

static uint64_t _hash(const char *key) {
  // FNV-1a hash (http://www.isthe.com/chongo/tech/comp/fnv/)
  size_t i;
  const size_t len = strlen(key);
  uint64_t h = 14695981039346656073ULL; // FNV_OFFSET 64-bit
  for (i = 0; i < len; ++i) {
    h = h ^ (unsigned char)key[i];
    h = h * 1099511628211ULL; // FNV_PRIME 64-bit
  }
  return h;
}

static int _rehash_set(hash_set_t *set) {
  size_t index;
  size_t i;
  int rc;
  for (i = 0; i < set->number_nodes; ++i) {
    if (set->nodes[i]) {
      if (!_get_index(set, set->nodes[i]->key, set->nodes[i]->hash, &index)) {
        // nodes already in the hash table must have indexes
        // this is a logic error that should never happen
        abort();
      }
      if (i != index) { // we are moving this node
        if (OK != (rc = _assign_node(set, set->nodes[i]->key,
                                     set->nodes[i]->hash, index))) {
          return rc;
        }
        _free_index(set, i);
      }
    }
  }
  return OK;
}

static int _set_add(hash_set_t *set, const char *key, uint64_t hash) {
  size_t index;
  if (_get_index(set, key, hash, &index)) {
    return OK;
  }
  // Expand nodes if we are close to our desired fullness
  if ((float)set->used_nodes / set->number_nodes > HASH_SET_LOAD_FACTOR) {
    size_t i;
    size_t orig_num_nodes = set->number_nodes;
    size_t num_nodes = orig_num_nodes << 1; // we want to double each time
    hash_set_node_t **tmp =
        realloc(set->nodes, num_nodes * sizeof(hash_set_node_t *));
    if (tmp == NULL || set->nodes == NULL) {
      return ALLOCATION_ERROR;
    }
    set->nodes = tmp;
    for (i = orig_num_nodes; i < num_nodes; ++i) {
      set->nodes[i] = NULL;
    }
    set->number_nodes = num_nodes;
    // rehash all nodes
    _rehash_set(set);
  }
  _assign_node(set, key, hash, index);
  ++set->used_nodes;
  return OK;
}

/*****************************************************************************
 * Public
 ****************************************************************************/

hash_set_t *hash_set_construct(void) {
  hash_set_t *hash_set_ptr = malloc(sizeof(hash_set_t));
  return hash_set_ptr;
}

int hash_set_initialize(hash_set_t *set, size_t num_nodes) {
  num_nodes = (num_nodes > 0) ? num_nodes : HASH_SET_DEFAULT_NUM_NODES;
  set->nodes = calloc(num_nodes, sizeof(hash_set_node_t *));
  if (set->nodes == NULL) {
    return ALLOCATION_ERROR;
  }
  set->number_nodes = num_nodes;
  set->used_nodes = 0;
  return OK;
}

void hash_set_clear(hash_set_t *set) {
  size_t i;
  for (i = 0; i < set->number_nodes; ++i) {
    if (set->nodes[i] != NULL) {
      _free_index(set, i);
    }
  }
  set->used_nodes = 0;
}

void hash_set_destroy(hash_set_t *set) {
  hash_set_clear(set);
  free(set->nodes);
  free(set);
}

int hash_set_add(hash_set_t *set, const char *key) {
  return _set_add(set, key, _hash(key));
}

bool hash_set_contains(const hash_set_t *set, const char *key) {
  size_t index;
  return _get_index(set, key, _hash(key), &index);
}

bool hash_set_remove(hash_set_t *set, const char *key) {
  size_t index;
  uint64_t hash = _hash(key);

  if (!_get_index(set, key, hash, &index)) {
    return false;
  }

  // remove this node
  _free_index(set, index);
  --set->used_nodes;
  return true;
}

size_t hash_set_count(const hash_set_t *set) { return set->used_nodes; }

char const *hash_set_next(const hash_set_t *set, size_t *index) {
  size_t i;

  for (i = *index; i < set->number_nodes; ++i) {
    if (set->nodes[i] != NULL) {
      *index = i + 1;
      return set->nodes[i]->key;
    }
  }

  *index = i;
  return NULL;
}
