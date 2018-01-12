#include"fjorge.h"

void error_tls(const char *afunc) {
  perror(afunc);

  exit(EXIT_FAILURE);
}
