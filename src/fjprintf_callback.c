#include"fjorge.h"

int fjprintf_callback(const char *fmt, ...) {
  register int ret = fputs(BADGE_CALLBACK, stderr);

  va_list arg;

  va_start(arg, fmt);

  ret += vfprintf(stderr, fmt, arg);

  va_end(arg);

  ret += fputs(CRLF, stderr);

  return ret;
}
