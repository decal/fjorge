#include"fjorge.h"

int fjputs_callback(const char *const emsg) {
  assert(emsg);

  register int ret = 0;

  if(vcmd->callback) {
    ret += fputs(BADGE_CALLBACK, stderr);
    ret += fputs(CYANF, stderr);
    ret += fputs(emsg, stderr);
    ret += fputs(RESET, stderr);
    ret += fputc('\n', stderr);
  }

  return ret;
}
