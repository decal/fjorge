#include"fjorge.h"

int fjputs_error(const char *const emsg) {
  assert(emsg);

  register int ret = 0;

  ret += fputs(BADGE_ERROR, stderr);
  ret += fputs(REDF, stderr);
  ret += fputs(emsg, stderr);
  ret += fputs(RESET, stderr);
  ret += fputc('\n', stderr);

  return ret;
}
