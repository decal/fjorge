#include"fjorge.h"

BIO *create_sockbio(const int sock) {
  assert(sock);

  BIO *restrict const biosok = BIO_new_fd(sock, BIO_FP_READ | BIO_FP_APPEND | BIO_CLOSE);

  if(!biosok)
    exit_verbose("BIO_new_fd", __FILE__, __LINE__);

  return biosok;
}
