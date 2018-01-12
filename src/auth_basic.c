#include"fjorge.h"

unsigned int auth_basic(const char *b64str) {
  const char *const auth_head = "Authorization: Basic ";
  char *restrict aret = malloc(1 + strlen(auth_head) + strlen(b64str));

  if(!aret)
    error_at_line(1, errno, __FILE__, __LINE__, "basic_auth: %s", strerror(errno));

  sprintf(aret, "%s%s", auth_head, b64str);
  
  return add_header(aret);
}

#if 0
int main(void) {
  const unsigned char *userpass="admin:admin";
  const size_t alen = strlen((const char*)userpass);
  const char *b64_userpass = encode_base64(userpass, alen);

  puts(b64_userpass);

  return 0;
}
#endif
