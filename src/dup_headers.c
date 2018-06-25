#include"fjorge.h"

void dup_headers(const char *anarg) {
  if(!anarg)
    puts(""); /* duplicate all */

  char *asep = strchr(anarg, ',');

  if(asep) {
    *asep = '\0';

    unsigned long acnt = strtoul(anarg, NULL, 10);

    if(errno == ERANGE && acnt == ULONG_MAX)
      exit_verbose("strtoul", __FILE__, __LINE__);
  } 
    
  return;
}
