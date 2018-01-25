#include"fjorge.h"

size_t count_portstr(const char *ports) {
  assert(ports);

  const PPORT_RANGELIST pport = parse_portstr(ports);

  return count_portlist(pport);
}
