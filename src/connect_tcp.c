#include"fjorge.h"

int connect_tcp(const char *ahost, const unsigned short aport) {
  assert(ahost);

  register signed int filedesc = 0;
  struct sockaddr_in inetaddr = { 0x00 }, connaddr = { 0x00, };
  struct addrinfo *inetinfo = calloc(1, sizeof *inetinfo);

  if(!inetinfo)
    return error_tcp("calloc");

  if(getnameinfo((struct sockaddr *)&inetaddr, sizeof inetaddr, (char *)ahost, strlen(ahost), NULL, 0, 0)) {
    //if(false)
    //  return error_tcp("getnameinfo");

    if(getaddrinfo(ahost, NULL, (const struct addrinfo *)NULL, (struct addrinfo **)&inetinfo))
      return error_tcp("getaddrinfo");
  }

  filedesc = socket(AF_INET, SOCK_STREAM, 0x0);

  if(filedesc < 0)
    return error_tcp("socket");

  memset(&connaddr, 0x00, sizeof connaddr);

  connaddr.sin_family = AF_INET;
  connaddr.sin_addr.s_addr = ((struct sockaddr_in *)inetinfo->ai_addr)->sin_addr.s_addr;
  connaddr.sin_port = htons(aport);

  fjprintf_verbose("Connecting to %s:%hu via TCP", vcmd->hostnam, vcmd->portnum);

  if(connect(filedesc, (const struct sockaddr *)&connaddr, sizeof connaddr) < 0)
    return error_tcp("connect");

  return filedesc;
}
