#include"fjorge.h"

char *strcat_glob(char **strs, const size_t size) {
  assert(strs);

  register size_t alen = 1;

  for(register int k = 0;k < size && strs[k];++k)
    alen += strlen(strs[k]);

  char *aret = calloc(1, alen);

  if(!aret)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  for(register int j = 0;j < size && strs[j];++j)
    strcat(aret, strs[j]);

  return aret;
}
