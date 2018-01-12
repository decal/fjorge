#include"fjorge.h"

int fjprintf_debug(const char *fmt, ...) {
  register int ret = 0;

  if(vcmd->debug) {
    va_list arg;

    va_start(arg, fmt);

    ret += vfprintf(stderr, fmt, arg);

    va_end(arg);
  }

  return ret;
}
