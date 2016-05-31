#include"fjorge.h"

signed char **print_hostnames(const char *restrict *const doms, const char *restrict *const x, size_t len) {
  register unsigned int i = 0;

  for (i = 0, --len; i <= len; i++) 
    if(i == len) 
      fprintf(stdout, "%s\n", doms[i]);
    else
      fprintf(stdout, "%s.", x[i]);

  return NULL;
}
