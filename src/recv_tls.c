#include"fjorge.h"

size_t recv_tls(BIO *sockfp) {
  static char rbuf[BUFSIZ] = { 0x00 };
  size_t acnt = 0, asiz = 0, alen = 0, cnln = 0, flag = 0, bret = 0;
  char *abuf = rbuf, *sptr = NULL;

  if(!sockfp) {
    fjputs_verbose("Encountered NULL FILE pointer before attempting to read plaintext response!");

    exit(EX_IOERR);
  }

  do
    bret = BIO_read(sockfp, rbuf, sizeof rbuf);
  while(BIO_should_retry(sockfp));

  if(bret <= 0)
    return bret;

  abuf = strtok_r(rbuf, CRLF, &sptr); 

  while(abuf) {
    if(!strncasecmp(abuf, "content-length:", 15)) {
      register const char *restrict c1 = strchr(abuf, ' ');

      cnln = atoi(++c1);
    }

   
    if(!strcmp(abuf, CRLF))
      flag = 1; 

    if(asiz) {
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

    if(vcmd->output)
      fputs(abuf, vcmd->output);

    if(!asiz && vcmd->brief) 
      break;

    acnt = strlen(abuf);
    asiz += acnt;

    if(flag) {
      alen += acnt;

      if(cnln && alen >= cnln)
        break;
    }

    abuf = strtok_r(NULL, CRLF, &sptr); 
  }

  return bret;
}
