#include"fjorge.h"

char *base64_decode (const char *asc_str) {
  assert(asc_str);

  const int asc_len = strlen(asc_str);
  register size_t b64_len = (((asc_len + 2) / 3) * 4);
  char *b64_str = malloc (++b64_len);

  if(!b64_str)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  if(EVP_DecodeBlock((unsigned char*)b64_str, (const unsigned char*)asc_str, asc_len) <= 0)
    error_at_line(1, errno, __FILE__, __LINE__, "EVP_DecodeBlock: %s", strerror(errno));

  b64_str[--b64_len] = '\0';

  return b64_str;
}

#ifdef TEST_DRIVE
/* gcc -DTEST_DRIVE -I../include -o base64_decode base64_decode.c -lcrypto -lssl */
int main(void) {
  char astr[]="YWJj";

  puts(base64_decode(astr));

  return 0;
}
#endif
