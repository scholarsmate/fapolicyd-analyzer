#ifndef SHA256_H
#define SHA256_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SHA256_DIGEST_SIZE 32            // SHA256 outputs a 32 byte digest

typedef struct sha256_ctx_struct sha256_ctx_t;

sha256_ctx_t * sha256_create();
void sha256_destroy(sha256_ctx_t * ctx);

void sha256_update(sha256_ctx_t * ctx, const unsigned char * data, size_t len);
void sha256_final(sha256_ctx_t * ctx, unsigned char * digest);  /* digest must be at least 32-bytes in length */

int sha256_file(char * hash_buf, size_t hash_buf_len, const char * path);

#ifdef __cplusplus
}
#endif

#endif   // SHA256_H
