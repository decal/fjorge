#include"fjorge.h"

int tls_send_request(BIO *abio, const HTTP_REQUEST *sreq) {
  static char amsg[BUFSIZ] = { 0x00, };

  sprintf(amsg, "%s %s %s" CRLF, sreq->verb, sreq->path, sreq->vers ? sreq->vers : HTT1);

  BIO_puts(abio, amsg);

  if(!vcmd->brief)
    fputs(amsg, stdout);

  if(vcmd->output)
    fputs(amsg, vcmd->output);

  if(sreq->host) {
    sprintf(amsg, "Host: %s" CRLF, sreq->host);

    BIO_puts(abio, amsg);
    
    if(!vcmd->brief)
      fputs(amsg, stdout);

    if(vcmd->output)
      fputs(amsg, vcmd->output);
  }

  BIO_puts(abio, CRLF);

  if(!vcmd->brief) 
    fputs(CRLF, stdout);

  if(vcmd->output)
    fputs(CRLF, vcmd->output);

  return 1;
}
