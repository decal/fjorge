#include"fjorge.h"

int fjputs_debug(const char *emsg) {
  register int ret = 0;

  if(vcmd->debug) {
    ret += fputs(BADGE_DEBUG, stderr);
    ret += fputs(emsg, stderr);
    ret += fputs(CRLF, stderr);
  }

  return ret;
}
