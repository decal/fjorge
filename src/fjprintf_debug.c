#include"fjorge.h"

int fjprintf_debug(const char *fmt, ...) {
  assert(fmt);

  register int ret = 0;

  if(vcmd->debug) {
    ret += fputs(BADGE_ERROR, stderr);

    va_list arg;

    va_start(arg, fmt);

    ret += fputs(WHITEF, stderr);
    ret += vfprintf(stderr, fmt, arg);

    va_end(arg);

    ret += fputs(RESET, stderr);
    ret += fputc('\n', stderr);
  }

  return ret;
}
