#include"fjorge.h"

int fjprintf_verbose(const char *fmt, ...) {
  register int ret = 0;

  if(vcmd->verbose) {
    va_list arg;

    va_start(arg, fmt);

    ret += fputs(BADGE_VERBOSE, stdout);
    ret += vfprintf(stdout, fmt, arg);
    ret += fputs(CRLF, stdout);

    va_end(arg);
  }

  return ret;
}
