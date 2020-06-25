/*****************************************************************************
 * TESTING
 ****************************************************************************/

#include <lib/util/rpm.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char * cptr;
    array_list_t * list = array_list_construct();
    array_list_initialize(list, 0);
    get_system_packages(list);

    while((cptr = array_list_pop(list))) {
        printf("%s\n", cptr);
        free(cptr);
    }

    array_list_destroy(list);
}