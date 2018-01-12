#include"fjorge.h"

int tcp_close(const int adesc) {
  if(shutdown(adesc, SHUT_RDWR) < 0) {
    error_at_line(0, errno, __FILE__, __LINE__, "shutdown: %s", strerror(errno));

    return 1;
  }

  if(close(adesc) < 0) {
    error_at_line(0, errno, __FILE__, __LINE__, "shutdown: %s", strerror(errno));

    return 2;
  }

  return 0;
}
