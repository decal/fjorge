#include"fjorge.h"

int fjputs_debug(const char *const emsg) {
  assert(emsg);

  register int ret = 0;

  if(vcmd->debug) {
    ret += fputs(BADGE_DEBUG, stderr);
    ret += fputs(WHITEF, stderr);
    ret += fputs(emsg, stderr);
    ret += fputs(RESET, stderr);
    ret += fputc('\n', stderr);
  }

  return ret;
}
