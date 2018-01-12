#include"fjorge.h"

int connect_tcp(const char *ahost, const unsigned short aport) {
  register signed int filedesc = 0;
  struct sockaddr_in inetaddr = { 0x00 }, connaddr = { 0x00, };
  struct addrinfo *inetinfo = calloc(1, sizeof inetinfo);

  if(!inetinfo)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  if(getnameinfo((struct sockaddr *)&inetaddr, sizeof inetaddr, (char *)ahost, strlen(ahost), NULL, 0, 0) < 0) {
    if(0)
      error(0, errno, "getnameinfo: %s", strerror(errno));

    if(getaddrinfo(ahost, NULL, (const struct addrinfo *)NULL, (struct addrinfo **)&inetinfo) < 0)
      error(1, errno, "getaddrinfo: %s", strerror(errno));
  }

  filedesc = socket(AF_INET, SOCK_STREAM, 0x0);

  if(filedesc < 0)
    error(1, errno, "socket: %s", strerror(errno));

  memset(&connaddr, '\0', sizeof connaddr);

  connaddr.sin_family = AF_INET;
  connaddr.sin_addr.s_addr = ((struct sockaddr_in *)inetinfo->ai_addr)->sin_addr.s_addr;
  connaddr.sin_port = htons(aport);

  fjprintf_verbose("Connecting to %s:%hu via TCP", vcmd->hostnam, vcmd->portnum);

  if(connect(filedesc, (const struct sockaddr *)&connaddr, sizeof connaddr) < 0)
    error(1, errno, "connect: %s", strerror(errno));

  return filedesc;
}
