#include"fjorge.h"

HTTP_VERSION *unpack_protover(const char *pv) {
  register char *aptr = NULL, *dlm0 = NULL, *dlm1 = NULL, achr = '\0';
  HTTP_VERSION *restrict const aret = calloc(1, sizeof *aret);

  if(!aret)
    exit_verbose("calloc", __FILE__, __LINE__);

  for(aptr = (char*)pv;*aptr;++aptr) 
    if(!isalpha(*aptr)) {
      achr = *aptr;

      *aptr = '\0';

      aret->proto = strdup(pv);

      if(!aret->proto)
        exit_verbose("strdup", __FILE__, __LINE__);

      *aptr = achr;

      for(dlm0 = dlm1 = aptr;*dlm1 && isalpha(*dlm1);++dlm1);

      achr = *dlm1;
      *dlm1 = '\0';

      aret->delim = strdup(dlm0);

      if(!aret->delim)
        exit_verbose("strdup", __FILE__, __LINE__);

      *dlm1 = achr;
      
      break;
    }

  for(aptr = ++dlm1;*aptr && *aptr != '.';aptr++);

  if(*aptr) {
    *aptr = '\0';
    aret->major = strdup(dlm1);

    if(!aret->major)
      exit_verbose("strdup", __FILE__, __LINE__);

    aret->minor = strdup(++aptr);

    if(!aret->minor)
      exit_verbose("strdup", __FILE__, __LINE__);

    *--aptr = '.';
  }

  return aret;
}
