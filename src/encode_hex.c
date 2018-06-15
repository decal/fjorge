#include"fjorge.h"

void encode_hex(unsigned char *readbuf, void *writebuf, const size_t len) {
  for(register size_t i = 0;i < len;++i) {
    char *l = (char *) (2 * i + ((intptr_t) writebuf));

    sprintf(l, "%02x", readbuf[i]);
  }

  return;
}
