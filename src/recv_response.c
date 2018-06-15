#include"fjorge.h"

size_t recv_response(BIO *sockfp) {
  char rbuf[BUFSIZ * 10] = { 0x00 };
  size_t acnt = 0, asiz = 0, alen = 0, cnln = 0, flag = 0, bret = 0;
  char *abuf = rbuf, *sptr = NULL;

  assert(sockfp);

  do {
    const int xret = BIO_read(sockfp, rbuf, sizeof rbuf);

    if(xret <= 0)
      return -1;

    bret += xret;
  } while(BIO_should_retry(sockfp));

  abuf = strtok_r(rbuf, CRLF, &sptr); 

  while(abuf && *abuf != '\n' && *abuf != '\r' && strcmp(abuf, CRLF) && strcmp(abuf, "0\r")) {
    if(!strncasecmp(abuf, "content-length:", 15)) {
      register const char *restrict c1 = strchr(abuf, ' ');

      /*** TODO: can this be an atoi() ? or should it be strtol() */
      cnln = atoi(++c1);

      if(!cnln)
        return -1;

      if(bret < cnln) {
_again: 
        sleep(0);

        if(!BIO_should_retry(sockfp))
          break;

        const int xret = BIO_read(sockfp, &rbuf[bret], sizeof rbuf - bret);

        if(xret <= 0)
          return -1;

        bret +=  xret;

        if(bret < cnln)
          goto _again;
      }
    }

    if(!strcmp(abuf, CRLF))
      flag = 1; 

    if(bret) {
      fputs(BADGE_RECV, stdout);
      puts(abuf);
    }

    if(!asiz && vcmd->verbose) {
      register char *const s1 = strchr(abuf, ' ');

      if(s1) {
        register char *const s2 = strchr(s1 + 1, ' ');

        if(s2) {
          register unsigned long int acod = strtoul(s1, NULL, 10);

          *s2 = '\0';
   
          acod = strtoul(s1, NULL, 10);

          if(errno != ERANGE && vcmd->verbose)
            fjprintf_verbose("HTTP response code: %lu", acod);
        }
      }
    }

    if(vcmd->output) {
      fputs(abuf, vcmd->output);
      fputs(CRLF, vcmd->output);
    }

    if(!asiz && vcmd->brief) 
      break;

    acnt = strlen(abuf);
    asiz += acnt;

    if(flag) {
      alen += acnt;

      if(cnln && alen >= cnln)
        break;

      if(!strcmp(abuf, CRLF))
        break;
    }

    abuf = strtok_r(NULL, CRLF, &sptr); 
  }

  return bret;
}
