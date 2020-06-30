/*****************************************************************************
 * TESTING
 ****************************************************************************/

#include <lib/util/array_list.h>
#include <stdio.h>

int main(int argc, char **argv) {
  char *value;
  array_list_t *list = array_list_construct();
  array_list_initialize(list, 0);
  array_list_push(list, "apples");
  array_list_push(list, "oranges");
  array_list_push(list, "pumpkin-pie");
  array_list_set(list, 1, "peaches");
  value = array_list_get(list, 1);
  printf("Value at index 1: %s\n", value);
  printf("array size: %lu\n", array_list_size(list));
  while ((value = array_list_pop(list))) {
    printf("%s\n", value);
  }
  array_list_destroy(list);
}
