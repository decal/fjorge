#include"fjorge.h"

unsigned short *array_ports(PORT_RANGELIST *plist) {
  register PPORT_RANGELIST aport = plist;

  assert(plist);

  size_t asize = count_ports(plist);

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
