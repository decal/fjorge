#include"fjorge.h"

size_t count_ports(PORT_NUMBERS *pnums) {
  register PPORT_NUMBERS pport = pnums;
  register size_t count = -1;

  assert(pnums);

  for(pport = pnums;pport;pport = pport->nextpn) {
    count += pport->porthi - pport->portlo;

    count++;
  }

  return count;
}
