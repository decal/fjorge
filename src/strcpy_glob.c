#include"fjorge.h"

char *strcpy_glob(char **strs, const size_t size) {
  assert(strs);

  if(!*strs)
    return NULL;

  register size_t acnt = 1;

  for(register int k = 0;k <= size;k++)
    acnt += strlen(strs[k]);

  char *aret = calloc(1, acnt);

  if(!aret)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  for(register int k = 0;k <= size;k++)
    strcat(aret, strs[k]);

  return aret;
}
