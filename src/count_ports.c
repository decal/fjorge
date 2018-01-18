#include"fjorge.h"

size_t count_ports(PORT_RANGELIST *pnums) {
  register PPORT_RANGELIST pport = pnums;
  register size_t count = -1;

  assert(pnums);

  for(pport = pnums;pport;pport = pport->nextpn) {
    count += pport->porthi - pport->portlo;

    count++;
  }

  return count;
}
