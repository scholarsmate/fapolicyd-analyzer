/*****************************************************************************
 * TESTING
 ****************************************************************************/

#include <lib/util/hash_table.h>
#include <stdio.h>

void emit_table(const hash_table_t *table, FILE *fp) {
  size_t index = 0;
  const char *key;
  char *value;

  while ((key = hash_table_next(table, &index, (void **)&value))) {
    fprintf(fp, "%s -> %s\n", key, value);
  }
}

int main(int argc, char **argv) {
  hash_table_t *table = hash_table_construct();

  hash_table_initialize(table, 0);
  hash_table_add(table, "fruit", "orange");
  hash_table_add(table, "vegetable", "raddish");

  printf("Table contains %lu nodes\n", hash_table_count(table));
  if (hash_table_contains(table, "fruit")) {
    printf("Table contains 'fruit'!\n");
  } else {
    printf("Table does not contain 'fruit'!\n");
  }

  if (hash_table_contains(table, "candy")) {
    printf("Table contains 'candy'!\n");
  } else {
    printf("Table does not contain 'candy'!\n");
  }

  emit_table(table, stdout);
  hash_table_add(table, "fruit", "lime");
  emit_table(table, stdout);

  hash_table_destroy(table);
}