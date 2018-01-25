#include"fjorge.h"

unsigned short *array_portlist(PORT_RANGELIST *plist) {
  assert(plist);

  register PPORT_RANGELIST aport = plist;
  size_t asize = count_portlist(plist);

  unsigned short *aretn = calloc(++asize, sizeof *aretn);

  if(!aretn) 
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  register unsigned short *ipush = aretn;

  while(aport) {
    unsigned short anint = aport->portlo;

    while(anint <= aport->porthi)
      *ipush++ = anint++;

    aport = aport->nextpn;
  }

  *ipush = 0;

  return aretn;
}
