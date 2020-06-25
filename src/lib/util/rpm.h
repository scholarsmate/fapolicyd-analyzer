/*
 * rpm.h
 */

#ifndef RPM_H
#define RPM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lib/util/array_list.h>

int get_system_packages(array_list_t *list);

#ifdef __cplusplus
}
#endif

#endif /* RPM_H */
