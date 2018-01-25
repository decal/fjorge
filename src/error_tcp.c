#include"fjorge.h"

int error_tcp(const char *afunc) {
  assert(afunc);

  perror(afunc);

  return 0;
}
