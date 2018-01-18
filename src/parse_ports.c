#include"fjorge.h"

static void fatal(const char *s) {
  perror(s);

  exit(EXIT_FAILURE);
}

PORT_NUMBERS *parse_ports(const char *portstr) {
  PORT_NUMBERS *headp = NULL, *portp = NULL;
  const char *current_range = portstr;
  char *endptr = NULL;
  long int rangestart = 0, rangeend = 0;

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

      if(rangestart == ERANGE)
        error_at_line(1, errno, __FILE__, __LINE__, "strtol: %s", strerror(errno));

      if (rangestart < 0 || rangestart > 65535) 
        fatal("Parse error: Ports must be between 0 and 65535 inclusive");

      current_range = endptr;

      while(isspace((int)(unsigned char)*current_range)) 
        current_range++;
    } else {
      fatal("Parse error: An example of proper portlist form is \"21-25,53,80\"");
    }

    /* Now I have a rangestart, time to go after rangeend */
    if (!*current_range || *current_range == ',') {
      /* Single port specification */
      rangeend = rangestart;
    } else if (*current_range == '-') {
      current_range++;

      if (isdigit((int) (unsigned char) *current_range)) {
        rangeend = strtol(current_range, &endptr, 10);

        if(rangeend == ERANGE)
          error_at_line(1, errno, __FILE__, __LINE__, "strtol: %s", strerror(errno));

        if (rangeend < 0 || rangeend > 65535 || rangeend < rangestart) 
          fatal("Parse error: Ports must be between 0 and 65535 inclusive");
        
        current_range = endptr;
      } else {
        fatal("Parse error: An example of proper portlist form is \"21-25,53,80\"");
      }
    } else {
      fatal("Parse error: An example of proper portlist form is \"21-25,53,80\"");
    }

    /* Now I have a rangestart and a rangeend, so I can add these ports */
    portp->portlo = (unsigned short)rangestart;

    while(rangestart <= rangeend)
      rangestart++;

    portp->porthi = (unsigned short)rangeend;
    portp->nextpn = calloc(1, sizeof *(portp->nextpn));

    if(!portp->nextpn)
      error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

    portp = portp->nextpn;

    /* Find the next range */
    while(isspace((int) (unsigned char) *current_range)) 
      current_range++;

    if (*current_range && *current_range != ',') 
      fatal("Parse error: An example of proper portlist form is \"21-25,53,80\"");
    
    if (*current_range == ',')
      current_range++;
  } while(current_range && *current_range);

  return headp;
}
