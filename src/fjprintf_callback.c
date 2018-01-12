#include"fjorge.h"

int fjprintf_callback(const char *fmt, ...) {
  register int ret = 0;
  va_list arg;

  va_start(arg, fmt);

  ret += fputs(BADGE_CALLBACK, stdout);
  ret += vfprintf(stdout, fmt, arg);
  ret += fputs(CRLF, stdout);

  va_end(arg);

  return ret;
}
