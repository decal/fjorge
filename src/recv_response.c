#include"fjorge.h"

size_t recv_response(FILE *sockfp) {
  static char abuf[BUFSIZ] = { 0x00 };
  register size_t acnt = 0, asiz = 0, alen = 0, cnln = 0, flag = 0;

  if(!sockfp) {
    fjputs_error("Encountered NULL FILE pointer before attempting to read plaintext response!");

    exit(EX_IOERR);
  }

  while(fgets(abuf, sizeof abuf, sockfp)) {
    if(!strncasecmp(abuf, "content-length:", 15)) {
      register const char *restrict c1 = strchr(abuf, ' ');

      cnln = atoi(++c1);
    }

    if(!strcmp(abuf, CRLF))
      flag = 1;

   
    if(asiz) {
      fputs(BADGE_RECV, stdout);
      fputs(abuf, stdout);
    }

    if(!asiz && vcmd->verbose) {
      register const char *const s1 = strchr(abuf, ' ');

      if(s1) {
        register char *const s2 = strchr(s1 + 1, ' ');

        if(s2) {
          register long unsigned int acod = 0;

          *s2 = '\0';
   
          acod = strtoul(s1, NULL, 10);

          if(errno != ERANGE)
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
  }

  return asiz;
}
