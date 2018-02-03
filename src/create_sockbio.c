#include"fjorge.h"

BIO *create_sockbio(const int sock, const HTTP_REQUEST *sreq) {
  assert(sock);
  assert(sreq);

  BIO *restrict const biosok = BIO_new_fd(sock, BIO_FP_READ | BIO_FP_APPEND | BIO_CLOSE);

  if(!biosok)
    error_at_line(1, errno, __FILE__, __LINE__, "BIO_new_fp: %s", strerror(errno));

  return biosok;
}
