#include"fjorge.h"

#ifdef TEST_DRIVE
#define fjprintf_error printf
#endif

static void error_port(const char *s) {
  fputs(BADGE_PORT, stderr);
  fputs(s, stderr);
  fputc('\n', stderr);

  fputs(BADGE_PORT, stderr);
  fputs("Examples of proper port-range,list format:\n", stderr);
  fputs("22\n", stderr);
  fputs("80,443\n", stderr);
  fputs("21-25\n", stderr);
  fputs("8000-8080,10000\n", stderr);
  fputs("1000-1100,2000-2100\n", stderr);
  fputc('\n', stderr);

  exit(EX_DATAERR);
}

PORT_RANGELIST *parse_portstr(const char *portstr) {
  PORT_RANGELIST *headp = NULL, *portp = NULL;
  const char *current_range = portstr;
  char *endptr = NULL;
  long int rangestart = 0, rangeend = 0, rangecur = 0, rangeprev = 0;

  assert(portstr);

  headp = calloc(1, sizeof *headp);

  if(!headp)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  portp = headp;

  do {
    while(*current_range && isspace((int) (unsigned char) *current_range)) 
      current_range++;

    if(isdigit((int) (unsigned char) *current_range)) {
      rangestart = strtol(current_range, &endptr, 10);

      if(errno == ERANGE)
        error_at_line(1, errno, __FILE__, __LINE__, "strtol: %s", strerror(errno));

      if (rangestart < 0 || rangestart > 65535) 
        error_port("Ports must be between 0 and 65535 inclusive");

      if(rangecur && rangecur == rangestart) {
        fjprintf_error("Duplicate port number detected: %lu", rangecur);

        exit(EX_DATAERR);
      }

      if(rangeprev && rangestart <= rangeprev) {
        fjprintf_error("Previous range end %lu is less than or equal to next range start %lu", rangeprev, rangestart);

        exit(EX_DATAERR);
      }

      current_range = endptr;

      while(isspace((int)(unsigned char)*current_range)) 
        current_range++;
    }

    if (!*current_range || *current_range == ',') {
      /* Single port specification */
      rangeend = rangestart;
    } else if (*current_range == '-') {
      current_range++;

      if (isdigit((int) (unsigned char) *current_range)) {
        rangeend = strtol(current_range, &endptr, 10);

        if(errno == ERANGE)
          error_at_line(1, errno, __FILE__, __LINE__, "strtol: %s", strerror(errno));

        if (rangeend < 0 || rangeend > 65535 || rangeend < rangestart) 
          error_port("Ports must be between 0 and 65535 inclusive");
        
        current_range = endptr;
      } else {
        error_port("End of range must start with a digit!");
      }
    } else {
      error_port("Range numbers must be separated by commas or dashes!");
    }

    if(rangecur) {
      if(rangecur > rangeend) {
        fjprintf_error("Current range %lu greater than ending range %lu", rangecur, rangeend);

        exit(EX_DATAERR);
      } else {
        if(rangecur == rangeend) {
          fjprintf_error("Current range: %lu equal to ending range %lu", rangecur, rangeend);

          exit(EX_DATAERR);
        }
      }
    }

    rangecur = rangeend;
    rangeprev = rangeend;

    /* Now I have a rangestart and a rangeend, so I can add these ports */
    portp->portlo = (unsigned short)rangestart;
    portp->porthi = (unsigned short)rangeend;
    portp->nextpn = calloc(1, sizeof *(portp->nextpn));

    if(!portp->nextpn)
      error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

    portp = portp->nextpn;

    /* Find the next range */
    while(isspace((int) (unsigned char) *current_range)) 
      current_range++;

    if (*current_range && *current_range != ',') 
      error_port("Parse error: range specifiers must be delimited by commas!");
    
    if (*current_range == ',')
      current_range++;
  } while(current_range && *current_range);

  return headp;
}

/* gcc -o parse_ports parse_ports.c count_ports.c array_ports.c -I../include -DTEST_DRIVE */
#ifdef TEST_DRIVE
int main(int argc, char *argv[1]) {
  register PPORT_RANGELIST p = NULL;

  if(argc < 2) {
    fprintf(stderr, "usage: %s PORTRANGE\n", *argv);
    fputs("  PORTRANGE  range of port numbers\n", stderr);
    fprintf(stderr, "ex. %s 1-10\n", *argv);
    exit(EX_USAGE);
  }

  p = parse_portstr(argv[1]);

  unsigned short *i = array_portlist(p);

  register unsigned short *ip = i;

  while(*ip) {
    printf("%d\n", *ip);
    
    ip++;
  }


  /* while(p) {
    if(p->portlo && p->porthi)
      printf("portlo: %u porthi: %u\n", p->portlo, p->porthi);

    p = p->nextpn;
  } */

  return 0;
}
#endif
