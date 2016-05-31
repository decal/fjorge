#include"fjorge.h"

void tls_error(const char *afunc) {
  perror(afunc);

  exit(EXIT_FAILURE);
}
