#include"fjorge.h"

int error_callback(const unsigned long ernum, const char *label) {
  assert(label);

  fputs(BADGE_CALLBACKERROR, stderr);
  fputs(label, stderr);
  fputs(": ", stderr);

  if(ernum) {
    const char *estr = ERR_reason_error_string(ernum);

    fputs(estr, stderr);
    fputc('\n', stderr);
  } else {
    fputs(strerror(errno), stderr);
    fputc('\n', stderr);

    return errno;
  }

  return ernum;
}
