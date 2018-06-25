#include"fjorge.h"

char *encode_base64 (const char *asc_str) {
  assert(asc_str);

  const size_t asc_len = strlen(asc_str);
  const size_t b64_len = (((asc_len + 2) / 3) * 4);
  char *b64_str = malloc (1 + b64_len);

  if(!b64_str)
    exit_verbose("malloc", __FILE__, __LINE__);

  if(EVP_EncodeBlock((unsigned char*)b64_str, (const unsigned char*)asc_str, asc_len) <= 0)
    exit_verbose("EVP_EncodeBlock", __FILE__, __LINE__);

  b64_str[b64_len] = '\0';

  return b64_str;
}

#ifdef TEST_DRIVE
int main(void) {
  char astr[] = "abc123";

  puts(encode_base64(astr));

  return 0;
}
#endif
