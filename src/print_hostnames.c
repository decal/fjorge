#include"fjorge.h"

signed char **print_hostnames(const char *restrict *const doms, const char *restrict *const x, size_t len) {
  assert(doms);
  assert(x);

  register unsigned int i = 0;

  for (i = 0, --len; i <= len; i++) 
    if(i == len) 
      puts(doms[i]);
    else
      printf("%s.", x[i]);

  return NULL;
}
