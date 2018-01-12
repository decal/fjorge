#include"fjorge.h"

void error_callback(const unsigned long err, const char *const label) {
  const char *const str = ERR_reason_error_string(err);

  assert(label);

  if(str)
    fputs(str, stderr);
  else
    fjprintf_error("%s failed: %lu (0x%lx)", label, err, err);

  fputc('\n', stderr);

  return;
}
