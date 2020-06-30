#include "rpm.h"
#include "detail/macros.h"
#include "error_codes.h"
#include <lib/util/sha256.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

table_t *package_get_rpm_info(void) {
  static const char *command = "/usr/bin/rpm -qa --queryformat \""
                               "%{NVRA}\t%{NAME}\t%{VERSION}\t%{RELEASE}\t%{ARCH}\t"
                               "%{SIZE}\t%{SHA256HEADER}\t%{BUILDTIME}\t%{INSTALLTIME}\t%{GROUP}\t"
                               "%{URL}\t%{RPMVERSION}\t%{SUMMARY}\t"
                               "%|DSAHEADER?{%{DSAHEADER:pgpsig}}:{%|RSAHEADER?{%{RSAHEADER:pgpsig}}:{%|"
                               "SIGGPG?{%{SIGGPG:pgpsig}}:{%|SIGPGP?{%{SIGPGP:pgpsig}}:{(none)}|}|}|}|"
                               "\n\"";
  static const char *column_names[] = {"NVRA", "NAME",         "VERSION",   "RELEASE",     "ARCH",
                                       "SIZE", "SHA256HEADER", "BUILDTIME", "INSTALLTIME", "GROUP",
                                       "URL",  "RPMVERSION",   "SUMMARY",   "SIGNATURE"};
  static const size_t column_count = sizeof(column_names) / sizeof(const char *);
  const char *record[column_count];

  table_t *table_ptr = NULL;
  FILE *fp = popen(command, "r");

  if (fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *probe = NULL;
    char *cptr = NULL;
    size_t col_num = 0;

    CHECK_PTR(table_ptr = table_construct());
    table_initialize(table_ptr, column_names, column_count);
    while ((read = getline(&line, &len, fp)) > 0) {
      if (line[read - 1] == '\n') {
        line[read - 1] = '\0';
      }
      cptr = line;
      for (col_num = 0; col_num < column_count - 1; ++col_num) {
        probe = strchr(cptr, '\t');
        *probe = '\0';
        record[col_num] = cptr;
        cptr = probe + 1;
      }
      record[col_num] = cptr;
      table_append_row(table_ptr, (const char **)record);
    }
    free(line);
    pclose(fp);
  }
  return table_ptr;
}

table_t *package_get_rpm_files_info(const table_t *rpm_table_ptr, package_progress_callback_t progress_callback,
                                    void *progress_client_ptr) {
  size_t nvra_col_num = 0;
  const size_t row_count = table_get_row_count(rpm_table_ptr);
  size_t i = 0;
  const char *command_pfx = "/usr/bin/rpm -ql --dump ";
  table_t *table_ptr = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *probe = NULL;
  char *cptr = NULL;
  size_t col_num = 0;

  static const char *column_names[] = {"NVRA", "Path",  "Size",    "Field_3", "SHA256",   "Permissions",
                                       "User", "Group", "Field_8", "Field_9", "Field_10", "Field_11"};
  static const size_t column_count = sizeof(column_names) / sizeof(char *);
  const char *record[column_count];

  CHECK_PTR(table_ptr = table_construct());
  table_initialize(table_ptr, column_names, column_count);

  if (OK != table_find_column_number(rpm_table_ptr, "NVRA", &nvra_col_num)) {
    abort();
  };

  for (i = 0; i < row_count; ++i) {
    char const *nvra = table_get_value(rpm_table_ptr, nvra_col_num, i);
    const size_t command_buf_len = strlen(command_pfx) + strlen(nvra) + 1;
    char *command_buf = NULL;
    FILE *fp = NULL;

    CHECK_PTR(command_buf = calloc(command_buf_len, sizeof(char)));
    snprintf(command_buf, command_buf_len, "%s%s", command_pfx, nvra);
    CHECK_PTR(fp = popen(command_buf, "r"));
    free(command_buf);

    while ((read = getline(&line, &len, fp)) > 0) {
      if (line[0] == '(') { /* (contains no file) */
        continue;
      }
      if (line[read - 1] == '\n') {
        line[read - 1] = '\0';
      }
      record[0] = nvra;
      cptr = line;
      for (col_num = 1; col_num < column_count - 1; ++col_num) {
        probe = strchr(cptr, ' ');
        *probe = '\0';
        record[col_num] = cptr;
        cptr = probe + 1;
      }
      record[col_num] = cptr;
      table_append_row(table_ptr, (const char **)record);
    }
    pclose(fp);
    if (progress_callback && 0 != progress_callback(progress_client_ptr, row_count, i + 1)) {
      break;
    }
  }
  free(line);
  return table_ptr;
}

/* Needs to be run as root to read much of the system files */
table_t *package_hash_files(const table_t *rpm_files_info_table_ptr, package_progress_callback_t progress_callback,
                            void *progress_client_ptr) {
  const size_t row_count = table_get_row_count(rpm_files_info_table_ptr);
  size_t file_path_col_num = 0;
  size_t nvra_col_num = 0;
  size_t i = 0;
  char hash[80];
  char buf[80];
  static const char *column_names[] = {"NVRA", "Path", "Size", "SHA256"};
  static const size_t column_count = sizeof(column_names) / sizeof(const char *);
  const char *record[column_count];
  table_t *table_ptr = NULL;

  if (OK != table_find_column_number(rpm_files_info_table_ptr, "Path", &file_path_col_num)) {
    abort();
  };
  if (OK != table_find_column_number(rpm_files_info_table_ptr, "NVRA", &nvra_col_num)) {
    abort();
  };
  CHECK_PTR(table_ptr = table_construct());
  table_initialize(table_ptr, column_names, column_count);

  record[2] = buf;
  record[3] = hash;
  for (i = 0; i < row_count; ++i) {
    record[0] = table_get_value(rpm_files_info_table_ptr, nvra_col_num, i);
    const char *file_path = record[1] = table_get_value(rpm_files_info_table_ptr, file_path_col_num, i);
    struct stat st;
    if (0 != access(file_path, R_OK) || 0 != stat(file_path, &st)) {
      continue;
    }
    if (S_ISREG(st.st_mode) && 0 < st.st_size && OK == sha256_file(hash, sizeof(hash), file_path)) {
      snprintf(buf, sizeof(buf) - 1, "%lld", st.st_size);
      table_append_row(table_ptr, (const char **)record);
    }
    if (progress_callback && 0 != progress_callback(progress_client_ptr, row_count, i + 1)) {
      break;
    }
  }
  return table_ptr;
}
