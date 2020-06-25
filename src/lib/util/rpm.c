#include "rpm.h"
#include "error_codes.h"
#include <stdio.h>
#include <stdlib.h>

int get_system_packages(array_list_t *list) {
  FILE *fp = popen("/usr/bin/rpm -qa", "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *cptr;

  if (fp) {
    while ((read = getline(&line, &len, fp)) > 0) {
      cptr = strdup(line);
      if (cptr) {
        array_list_push(list, cptr);
      }
    }
    free(line);
    return OK;
  }
  return ALLOCATION_ERROR;
}
