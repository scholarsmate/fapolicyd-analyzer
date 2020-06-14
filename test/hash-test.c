#include <stdio.h>
#include <lib/util/hash.h>

int main(void)
{
    char hash[80];
    sha256_file(hash, sizeof(hash), "/etc/passwd");
    printf("hash: %s\n", hash);
    return 0;
}
