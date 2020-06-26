/*****************************************************************************
 * TESTING
 ****************************************************************************/

#include <lib/util/table.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
     const char * column_names[] = {
        "column_1", "column_2", "column_3", "column_4", "column_5",
        "column_6", "column_7", "column_8", "column_9", "column_10"
    };
    size_t column_count = sizeof(column_names) / sizeof(const char *);
    size_t row_count = 100000;
    table_t * table_ptr = table_construct();

    table_initialize(table_ptr, column_names, column_count);

    if (row_count) {
        size_t i = 0, j = 0;
        char * row_data[column_count];
        char buf[column_count][1024];
        table_index_t * file_name_index = NULL;
        const row_index_t * row_index = NULL;
    
        for (i = 0; i < row_count; ++i) {
            for(j = 0; j < column_count; ++j) {
                snprintf(buf[j], sizeof(*buf) - 1, "row_data %lu", (i * column_count + j)%10001);
                row_data[j] = buf[j];
            }
            table_append(table_ptr, (const char **)row_data);
        }
        if(column_count != table_get_column_count(table_ptr)) {
            fprintf(stderr, "column_count %lu should equal %lu\n", table_get_column_count(table_ptr), row_count);
            abort();
        }
        if (row_count != table_get_row_count(table_ptr)) {
            fprintf(stderr, "row_count %lu should equal %lu\n", table_get_row_count(table_ptr), row_count);
            abort();
        }
        printf("row\t");
        for (i = 0; i < column_count; ++i) {
            printf("%s\t", table_get_column_name(table_ptr, i));
        }
        printf("\n");
        for (i = 0; i < row_count; ++i) {
            printf("%lu\t", i);
            for(j = 0; j < column_count; ++j) {
                printf("%s\t", table_get_value(table_ptr, j, i));
            }
            printf("\n");
        }
        printf("row_count %lu column_count %lu\n", table_get_row_count(table_ptr), table_get_column_count(table_ptr));
        file_name_index = table_index_create(table_ptr, 1);
        printf("items in index: %lu\n", table_index_count(file_name_index));
        row_index = table_index_find(file_name_index, "row_data 17");
        if (row_index) {
            size_t * index_p = NULL;
            while((index_p=row_index_next(row_index, index_p))) {
                printf("%lu", *index_p);
                printf(" -> %s\n", table_get_value(table_ptr, 1, *index_p));
            }
        }
        table_index_destroy(file_name_index);
    }
    table_destroy(table_ptr);
}
