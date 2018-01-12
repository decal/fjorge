#include"fjorge.h"

int fjprintf_error(const char *fmt, ...) {
  register int ret = 0;
  va_list arg;

  va_start(arg, fmt);

  ret += fputs(BADGE_ERROR, stderr);
  ret += vfprintf(stderr, fmt, arg);
  ret += fputs(CRLF, stderr);

  va_end(arg);

  return ret;
}
