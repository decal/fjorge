#include"fjorge.h"

BIO *create_sockbio(const int sock, const HTTP_REQUEST *sreq) {
  assert(sock);
  assert(sreq);

  /* FILE *restrict const sockfp = fdopen(sock, "a+");

  if(!sockfp)
    error_at_line(1, errno, __FILE__, __LINE__, "fdopen: %s", strerror(errno)); */

  BIO *restrict const biosok = BIO_new_fd(sock, 0);

  if(!biosok)
    error_at_line(1, errno, __FILE__, __LINE__, "BIO_new_fp: %s", strerror(errno));

  return biosok;
}
