#include"fjorge.h"

int fjputs_verbose(const char *const emsg) {
  register int ret = 0;

  if(vcmd->verbose) {
    ret += fputs(BADGE_VERBOSE, stdout);
    ret += fputs(emsg, stdout);
    ret += fputs(CRLF, stdout);
  }

  return ret;
}
