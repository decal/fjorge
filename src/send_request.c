#include"fjorge.h"

int send_request(BIO *abio, const HTTP_REQUEST *sreq) {
  char amsg[BUFSIZ] = { 0x00, };

  sprintf(amsg, "%s %s %s" CRLF, sreq->verb, sreq->path, sreq->vers ? sreq->vers : HTT1);

  register int slen = BIO_puts(abio, amsg); 

  if(!vcmd->brief) {
    // fputs(BADGE_SEND, stdout);
    // fputs(amsg, stdout);
  }

  if(vcmd->output)
    fputs(amsg, vcmd->output);

  if(sreq->host) {
    sprintf(amsg, "Host: %s" CRLF, sreq->host);

    slen += BIO_puts(abio, amsg);
    
    if(!vcmd->brief) {
      // fputs(BADGE_SEND, stdout);
      // fputs(amsg, stdout);
    }

    if(vcmd->output)
      fputs(amsg, vcmd->output);
  } else {
    if(vcmd->portstr)
      sprintf(amsg, "Host: %s:%s" CRLF, vcmd->hostnam, vcmd->portstr);
    else
      sprintf(amsg, "Host: %s" CRLF, vcmd->hostnam);

    slen += BIO_puts(abio, amsg);

    if(!vcmd->brief) {
      // fputs(BADGE_SEND, stdout);
      // fputs(amsg, stdout);
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
        // fputs(BADGE_SEND, stdout);
        // fputs(lsp->header, stdout);
        // fputc('\n', stdout);
      }

      if(vcmd->output) {
        fputs(lsp->header, vcmd->output);
        fputc('\n', vcmd->output);
      }

      lsp = lsp->next;
    } while(lsp);
  } 

  slen += BIO_puts(abio, CRLF);

  if(BIO_flush(abio) < 1)
    exit_verbose("BIO_flush", __FILE__, __LINE__);

  if(!vcmd->brief) {
    // fputs(BADGE_SEND, stdout);
    fputc('\n', stdout);
  }

  if(vcmd->output)
    fputc('\n', vcmd->output);

  return slen;
}
