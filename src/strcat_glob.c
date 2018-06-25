#include"fjorge.h"

char *strcat_glob(char **strs, const size_t size) {
  assert(strs);

  register size_t alen = 1;

  for(register int k = 0;k < size && strs[k];++k)
    alen += strlen(strs[k]);

  char *const aret = malloc(alen);

  if(!aret)
    exit_verbose("calloc", __FILE__, __LINE__);

  *aret = '\0';

  for(register int j = 0;j < size && strs[j];++j)
    strcat(aret, strs[j]);

  return aret;
}
