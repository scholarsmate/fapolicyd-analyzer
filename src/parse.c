#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int peek_fields(char *** fields_addr, char * buffer) {
    int num_fields = 0;
    char * probe = buffer;
    char ** fields;

    while (*probe) {
        if (*probe == '=') ++num_fields;
        ++probe;
    }
    *fields_addr = fields = (char**)malloc((num_fields +1) * sizeof(char *));
    fields[num_fields] = NULL;
    probe = buffer;
    num_fields = 0;
    fields[num_fields++] = probe;
    while (*probe) {
        if (*probe == ' ') {
            while (*probe == ' ') ++probe;
            if (*probe == ':') {
                ++probe;
                while (*probe == ' ') ++probe;
            }
            fields[num_fields++] = probe;
            continue;
        } 
        if (*probe == '=') {
            *probe = '\0';
        }
        ++probe;
    }
    return num_fields;
}


int parse(int num_fields, char ** fields, char ** values, char * buffer) {
    char * probe = buffer;
    int parsed_fields = 0;

    while (*probe) {
        if (*probe == '=') {
            *probe = '\0';
            if (0 != strcmp(fields[parsed_fields], buffer)) {
                return -1;
            }
            values[parsed_fields++] = ++probe;
            continue;
        }
        if (*probe == ' ') {
            *probe = '\0';
            ++probe;
            while (*probe == ' ') ++probe;
            if (*probe == ':') {
                ++probe;
                while (*probe == ' ') ++probe;
            }
            buffer = probe;
        }
        ++probe;
    }
    if (*(--probe) == '\n') *probe ='\0';
    return num_fields == parsed_fields ? 0 : -2;
}


void emit_record(int num_fields, char ** fields, char ** values) {
    int emitted_fields = 0;

    while (emitted_fields < num_fields) {
        printf("[%d] %s -> %s\n", emitted_fields +1, fields[emitted_fields], values[emitted_fields]);
        ++emitted_fields;
    }
    printf("\n");
}


int main(int argc, char * argv[]){
    FILE * fp;
    char * line = NULL;
    char * first_line;
    char ** fields;
    char ** values;
    int num_fields;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1],"r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    read = getline(&line, &len, fp);
    if (read < 0) {
        exit(EXIT_FAILURE);
    }
    first_line = strndup(line, len);
    num_fields = peek_fields(&fields, first_line);
    values = (char**)malloc((num_fields +1) * sizeof(char *));
    values[num_fields] = NULL;
    do {
        if (parse(num_fields, fields, values, line) !=0) {
            exit(EXIT_FAILURE);
        }
        emit_record(num_fields, fields, values);
    } while ((read = getline(&line, &len, fp)) != -1);

    fclose(fp);
    free(values);
    free(line);
    free(fields);
    free(first_line);
    exit(EXIT_SUCCESS);
}
