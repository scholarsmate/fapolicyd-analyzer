/*
 * hash_set.h
 */

#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * @brief Computes a SHA256 hash of the contents of the file pointed to by the
 * given path
 *
 * @param hash_buf character buffer to store the hexidecimal representation of
 * the hash
 * @param hash_buf_len the size of the character buffer
 * @param path path to the file whose contents will be hashed
 * @return int returns zero (OK) on success and non-zero otherwise
 */
int sha256_file(char *hash_buf, size_t hash_buf_len, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* HASH_H */
