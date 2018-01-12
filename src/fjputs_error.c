#include"fjorge.h"

int fjputs_error(const char *emsg) {
  register int ret = 0;

  ret += fputs(BADGE_ERROR, stderr);
  ret += fputs(emsg, stderr);
  ret += fputs(CRLF, stderr);

  return ret;
}
