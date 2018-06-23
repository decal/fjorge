#include"fjorge.h"

int fjprintf_callback(const char *fmt, ...) {
  assert(fmt);

  register int ret = 0;

  if(vcmd->callback) {
    ret += fputs(BADGE_CALLBACK, stderr);

    va_list arg;

    va_start(arg, fmt);

    ret += fputs(CYANF, stderr);
    ret += vfprintf(stderr, fmt, arg);

    va_end(arg);

    ret += fputs(RESET, stderr);
    ret += fputc('\n', stderr);
  }

  return ret;
}
