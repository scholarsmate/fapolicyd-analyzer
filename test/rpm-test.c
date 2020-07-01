/*****************************************************************************
 * TESTING
 ****************************************************************************/

#include <libfapolicyd-analyzer/rpm.h>
#include <stdio.h>
#include <stdlib.h>

int emit_progress(void *clientp, size_t total, size_t current) {
  FILE *fp = (FILE *)clientp;
  if (total && fp) {
    fprintf(fp, "%%%d\r", (int)(100 * current / total));
    fflush(fp);
  }
  return 0;
}

void print_table(const table_t *table_ptr, FILE *fp) {
  const size_t column_count = table_get_column_count(table_ptr);
  const size_t row_count = table_get_row_count(table_ptr);
  size_t i = 0, j = 0;

  fprintf(fp, "ROW_ID");
  for (i = 0; i < column_count; ++i) {
    fprintf(fp, "\t%s", table_get_column_name(table_ptr, i));
  }
  for (i = 0; i < row_count; ++i) {
    fprintf(fp, "\n%lu", i + 1);
    for (j = 0; j < column_count; ++j) {
      fprintf(fp, "\t%s", table_get_value(table_ptr, j, i));
    }
  }
  fprintf(fp, "\n");
}

int main(int argc, char **argv) {
  table_t *rpm_table_ptr = NULL;
  table_t *rpm_file_table_ptr = NULL;
  table_t *disk_file_table_ptr = NULL;

  rpm_table_ptr = package_get_rpm_info();
  print_table(rpm_table_ptr, stdout);
  printf("\n--------\n");

  rpm_file_table_ptr = package_get_rpm_files_info(rpm_table_ptr, emit_progress, stdout);
  print_table(rpm_file_table_ptr, stdout);
  printf("\n--------\n");

  disk_file_table_ptr = package_hash_files(rpm_file_table_ptr, emit_progress, stdout);
  print_table(disk_file_table_ptr, stdout);
  printf("\n--------\n");

  table_destroy(disk_file_table_ptr);
  table_destroy(rpm_file_table_ptr);
  table_destroy(rpm_table_ptr);

  return EXIT_SUCCESS;
}
