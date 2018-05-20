#include"fjorge.h"

int fjprintf_error(const char *fmt, ...) {
  assert(fmt);

  register int ret = fputs(BADGE_ERROR, stderr);

  va_list arg;

  va_start(arg, fmt);

  ret += fputs(REDF, stderr);
  ret += vfprintf(stderr, fmt, arg);

  va_end(arg);

  ret += fputs(RESET, stderr);
  ret += fputs(CRLF, stderr);

  return ret;
}
