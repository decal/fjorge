#include"fjorge.h"

void error_callback(const unsigned long err, const char *const label) {
  assert(label);

  const char *const str = ERR_reason_error_string(err);

  if(str)
    fjputs_error(str);
  else
    fjprintf_error("%s failed: %lu (0x%lx)", label, err, err);

  return;
}
