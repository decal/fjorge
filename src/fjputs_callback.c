#include"fjorge.h"

int fjputs_callback(const char *emsg) {
  register int ret = 0;

  if(vcmd->callback) {
    ret += fputs(BADGE_CALLBACK, stderr);
    ret += fputs(emsg, stderr);
    ret += fputs(CRLF, stderr);
  }

  return ret;
}
