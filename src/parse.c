#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Structure that holds the parse context
 * 
 */
typedef struct parse_context_struct {
    int num_fields;
    char ** fields;
    char ** values;
    char * _exemplar;
} parse_context_t;

/**
 * @brief Allocates and initializes a parse_context_t
 * 
 * @return parse_context_t* pointer to a new parse_context_t or NULL if allocation failed
 */
parse_context_t * construct_parse_context(void) {
    parse_context_t * parse_ctx_ptr = (parse_context_t *)malloc(sizeof(parse_context_t));
    if (parse_ctx_ptr) {
        parse_ctx_ptr->num_fields = 0;
        parse_ctx_ptr->fields = NULL;
        parse_ctx_ptr->values = NULL;
        parse_ctx_ptr->_exemplar = NULL;
    }
    return parse_ctx_ptr;
}

/**
 * @brief Destroys a parse_context_t
 * 
 * @param parse_ctx a constructed parse_context pointer
 */
void destroy_parse_context(parse_context_t * parse_ctx_ptr) {
    free(parse_ctx_ptr->fields);
    free(parse_ctx_ptr->values);
    free(parse_ctx_ptr->_exemplar);
    free(parse_ctx_ptr);
}

/**
 * @brief Embues the parser with the fields, order of fields and number of fields to parse and allocates internal data structures for parsing
 * 
 * @param parse_ctx a constructed parse_context pointer
 * @param line null-terminated line containing an exemplar record to embue into the parser context
 * @return int 0 = success, -1 = memory allocation faillure, -2 = no fields found in line
 */
int embue_parser(parse_context_t * parse_ctx_ptr, char * line) {
    int num_fields = 0;
    char * buffer = strdup(line);
    char * probe = buffer;
    char ** fields;
    char ** values;

    if (!buffer) {
        return -1;
    }
    while (*probe) {
        if (*probe == '=') ++num_fields;
        ++probe;
    }
    if (!num_fields) {
        return -2;
    }
    if (!(fields = (char**)malloc((num_fields + 1) * sizeof(char *)))) {
        free(buffer);
        return -1;
    }
    if (!(values = (char**)malloc((num_fields + 1) * sizeof(char *)))) {
        free(buffer);
        free(fields);
        return -1;
    }
    fields[num_fields] = NULL;
    values[num_fields] = NULL;
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
    parse_ctx_ptr->num_fields = num_fields;
    parse_ctx_ptr->fields = fields;
    parse_ctx_ptr->values = values;
    parse_ctx_ptr->_exemplar = buffer;
    return 0;
}

/**
 * @brief A shallow and destructive parser, but very efficient
 * 
 * @param parse_ctx_ptr an embued parse_context pointer
 * @param line null-terminated line containing a record
 * @return int 0 = success, -1 = fields are not in the order expected, -2 expected number of fields did not match the number of fields parsed
 */
int parse(parse_context_t * parse_ctx_ptr, char * line) {
    char * probe = line;
    int parsed_fields = 0;
    char ** fields = parse_ctx_ptr->fields;
    char ** values = parse_ctx_ptr->values;

    while (*probe) {
        if (*probe == '=') {
            *probe = '\0';
            if (0 != strcmp(fields[parsed_fields], line)) {
                return -1;  // field order does not match embued exemplar
            }
            values[parsed_fields++] = ++probe;  // the beginning of a new value
            continue;
        }
        if (*probe == ' ') {
            *probe = '\0';  // terminate the end of the value
            ++probe;
            while (*probe == ' ') ++probe;  // skip additional spaces
            // Skip a colon
            if (*probe == ':') {
                ++probe;
                while (*probe == ' ') ++probe;  // skip additional spaces after the colon
            }
            line = probe;  // the beginning of a new field
        }
        ++probe;
    }
    if (*(--probe) == '\n') *probe ='\0';  // gobble up trailing newlines
    return parse_ctx_ptr->num_fields == parsed_fields ? 0 : -2;
}

/**
 * @brief Emit the parsed record to stdout for debugging
 * 
 * @param parse_ctx_ptr a populated parse_context pointer
 */
void emit_record(parse_context_t * parse_ctx_ptr, FILE * file_ptr) {
    int emitted_fields = 0;
    int num_fields = parse_ctx_ptr->num_fields;

    while (emitted_fields < num_fields) {
        fprintf(file_ptr, "[%d] %s -> %s\n", emitted_fields + 1, parse_ctx_ptr->fields[emitted_fields], parse_ctx_ptr->values[emitted_fields]);
        ++emitted_fields;
    }
    fprintf(file_ptr, "\n");
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char * argv[]){
    FILE * fp = stdin;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    parse_context_t * parse_ctx_ptr;

    if (argc == 2) {
        if ((fp = fopen(argv[1],"r")) == NULL) {
            exit(EXIT_FAILURE);
        }
    } else if (argc != 1) {
        fprintf(stderr, "Illegal usage\n");
        exit(EXIT_FAILURE);
    }
    if ((read = getline(&line, &len, fp)) < 0) {
        exit(EXIT_FAILURE);
    }
    if(!(parse_ctx_ptr = construct_parse_context())) {
        exit(EXIT_FAILURE);
    }
    if (embue_parser(parse_ctx_ptr, line) != 0) {
        exit(EXIT_FAILURE);
    }
    do {
        if (parse(parse_ctx_ptr, line) !=0) {
            exit(EXIT_FAILURE);
        }
        emit_record(parse_ctx_ptr, stdout);
    } while ((read = getline(&line, &len, fp)) > 0);

    fclose(fp);
    free(line);
    destroy_parse_context(parse_ctx_ptr);
    exit(EXIT_SUCCESS);
}
