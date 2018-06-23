#include"fjorge.h"

HTTP_VERSION *unpack_protover(const char *pv) {
  register char *aptr = NULL, *dlm0 = NULL, *dlm1 = NULL, achr = '\0';
  HTTP_VERSION *aret = calloc(1, sizeof *aret);

  if(!aret)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  for(aptr = (char*)pv;*aptr;++aptr) 
    if(!isalpha(*aptr)) {
      achr = *aptr;

      *aptr = '\0';

      aret->proto = strdup(pv);

      if(!aret->proto)
        error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

      *aptr = achr;

      for(dlm0 = dlm1 = aptr;*dlm1 && isalpha(*dlm1);++dlm1);

      achr = *dlm1;
      *dlm1 = '\0';

      aret->delim = strdup(dlm0);

      if(!aret->delim)
        error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

      *dlm1 = achr;
      
      break;
    }

  for(aptr = ++dlm1;*aptr && *aptr != '.';aptr++);

  if(*aptr) {
    *aptr = '\0';
    aret->major = strdup(dlm1);

    if(!aret->major)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

    aret->minor = strdup(++aptr);

    if(!aret->minor)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

    *--aptr = '.';
  }

  return aret;
}
