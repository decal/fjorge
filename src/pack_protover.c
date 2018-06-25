#include"fjorge.h"

char *pack_protover(const HTTP_VERSION *aver) {
  assert(aver);

  const size_t alen = 2 + strlen(aver->proto) + strlen(aver->delim) + strlen(aver->major) + strlen(aver->minor);

  char *const aret = malloc(alen);

  if(!aret)
    exit_verbose("malloc", __FILE__, __LINE__);

  sprintf(aret, "%s%s%s.%s", aver->proto, aver->delim, aver->major, aver->minor);

  return aret;
}
