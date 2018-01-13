#include"fjorge.h"

int fjprintf_verbose(const char *fmt, ...) {
  register int ret = 0;

  if(vcmd->verbose) {
    ret += fputs(BADGE_VERBOSE, stdout);

    va_list arg;

    va_start(arg, fmt);

    ret += vfprintf(stdout, fmt, arg);

    va_end(arg);

    ret += fputs(CRLF, stdout);
  }

  return ret;
}
