#include"fjorge.h"

int send_tls(BIO *abio, const HTTP_REQUEST *sreq) {
  char amsg[BUFSIZ] = { 0x00, };

  sprintf(amsg, "%s %s %s" CRLF, sreq->verb, sreq->path, sreq->vers ? sreq->vers : HTT1);

  register int slen = BIO_puts(abio, amsg); 

  if(!vcmd->brief) {
    fputs(BADGE_SEND, stdout);
    fputs(amsg, stdout);
  }

  if(vcmd->output)
    fputs(amsg, vcmd->output);

  if(sreq->host) {
    sprintf(amsg, "Host: %s" CRLF, sreq->host);

    slen += BIO_puts(abio, amsg);
    
    if(!vcmd->brief) {
      fputs(BADGE_SEND, stdout);
      fputs(amsg, stdout);
    }

    if(vcmd->output)
      fputs(amsg, vcmd->output);
  }

  if(sreq->hdrs) {
    register PHEADER_LIST lsp = sreq->hdrs;

    do { 
      slen += BIO_puts(abio, lsp->header);
      slen += BIO_puts(abio, CRLF);

      if(!vcmd->brief) {
        fputs(BADGE_SEND, stdout);
        puts(lsp->header);
      }

      if(vcmd->output) {
        fputs(lsp->header, vcmd->output);
        fputs(CRLF, vcmd->output);
      }

      lsp = lsp->next;
    } while(lsp);
  }

  slen += BIO_puts(abio, CRLF);

  if(!vcmd->brief) {
    fputs(BADGE_SEND, stdout);
    fputs(CRLF, stdout);
  }

  if(vcmd->output)
    fputs(CRLF, vcmd->output);

  return slen;
}
