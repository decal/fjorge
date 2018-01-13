#include"fjorge.h"

const size_t array_length(char **anarr) {
  register char *const *pp = anarr;
  register size_t ll = 0;

  if(!(pp && *pp))
    return 0;

  do 
    ll++;
  while(*++pp);

  return ll;
}
