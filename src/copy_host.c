#include"fjorge.h"

char *copy_host(char **strs, const size_t size) {
  char *aret = NULL;
  size_t acnt = 1;

  for(register int k = 0;k < size;++k)
    acnt++;

  aret = calloc(1, acnt);

  if(!aret)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  for(register int j = 0;j < size;++j)
    strcat(aret, strs[j]);

  return aret;
}
