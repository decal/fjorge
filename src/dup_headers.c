#include"fjorge.h"

void dup_headers(const char *anarg) {
  if(!anarg)
    puts(""); /* duplicate all */

  const char *asep = strchr(anarg, ',');

  if(asep) {
    *asep++ = '\0';

    unsigned long acnt = strtoul(anarg, NULL, 10);

    if(acnt == ULONG_MAX && errno == ERANGE)
      error_at_line(1, errno, __FILE__, __LINE__, "strtoul: %s", strerror(errno));
  } 
    
  return;
}
