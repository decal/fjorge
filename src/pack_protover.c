#include"fjorge.h"

char *pack_protover(const HTTP_VERSION *aver) {
  size_t alen = 1 + strlen(aver->proto) + strlen(aver->delim) + strlen(aver->major) + strlen(aver->minor);

  char *aret = malloc(++alen);

  if(!aret)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  sprintf(aret, "%s%s%s.%s", aver->proto, aver->delim, aver->major, aver->minor);

  return aret;
}
