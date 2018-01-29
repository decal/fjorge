#include"fjorge.h"

char *parse_writebuf(char *wbuf) {
  register char *restrict wptr = wbuf;

  if(*wptr == '@') {
    wptr++;

    if(*wptr == '-') {
    }
  } 

  return wbuf;
}
