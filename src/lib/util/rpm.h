/*
 * rpm.h
 */

#ifndef RPM_H
#define RPM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lib/util/table.h>

typedef int (*package_progress_callback_t)(void *, size_t, size_t);

table_t *package_get_rpm_info(void);
table_t *
package_get_rpm_files_info(const table_t *rpm_table_ptr,
                           package_progress_callback_t progress_callback,
                           void *progress_client_ptr);
table_t *package_hash_files(const table_t *rpm_files_info_table_ptr,
                            package_progress_callback_t progress_callback,
                            void *progress_client_ptr);

#ifdef __cplusplus
}
#endif

#endif /* RPM_H */
