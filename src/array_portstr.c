#include"fjorge.h"

unsigned short *array_portstr(const char *astr) {
  assert(astr);

  PPORT_RANGELIST plist = parse_portstr(astr);
  
  return array_portlist(plist);
}
