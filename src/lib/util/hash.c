/*
 * hash.c
 */

#include "hash.h"
#include "error_codes.h"
#include <fcntl.h>
#include <gcrypt.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static int to_hex(char *hash_buf, size_t hash_buf_len,
                  const unsigned char *digest, size_t digest_len) {
  size_t i;
  if (hash_buf_len > (digest_len << 1)) {
    for (i = 0; i < digest_len; i++) {
      sprintf(hash_buf + (i << 1), "%02x", digest[i]);
    }
    return OK;
  }
  return RANGE_ERROR;
}

int sha256_file(char *hash_buf, size_t hash_buf_len, const char *path) {
  unsigned char buf[1024 * 4]; /* 4K reads */
  gcry_md_hd_t hd;
  size_t sz;
  int rc;
  int fd = open(path, O_RDONLY);

  if (fd < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  gcry_md_open(&hd, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);
  while ((sz = read(fd, buf, sizeof(buf))) > 0) {
    gcry_md_write(hd, buf, sz);
  }
  close(fd);
  rc = to_hex(hash_buf, hash_buf_len, gcry_md_read(hd, GCRY_MD_SHA256),
              gcry_md_get_algo_dlen(GCRY_MD_SHA256));
  gcry_md_close(hd);
  return rc;
}
