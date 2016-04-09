#include"fjorge.h"

FILE *send_request(const int sock, const HTTP_REQUEST *sreq) {
  register FILE *restrict const sockfp = fdopen(sock, "a+");

  if(!sockfp)
    error_at_line(1, errno, __FILE__, __LINE__, "fdopen: %s", strerror(errno));

  fprintf(sockfp, "%s %s %s" CRLF, sreq->verb, sreq->path, sreq->vers ? sreq->vers : HTT1);

  if(!vcmd->brief)
    fprintf(stdout, "%s %s %s" CRLF, sreq->verb, sreq->path, sreq->vers ? sreq->vers : HTT1);

  if(vcmd->output)
    fprintf(vcmd->output, "%s %s %s" CRLF, sreq->verb, sreq->path, sreq->vers ? sreq->vers : HTT1);

  if(sreq->host) {
    fprintf(sockfp, "Host: %s" CRLF, sreq->host);

    if(!vcmd->brief)
      fprintf(stdout, "Host: %s" CRLF, sreq->host);

    if(vcmd->output)
      fprintf(vcmd->output, "Host: %s" CRLF, sreq->host);
  }

  if(sreq->hdrs) {
    register PLINKED_LIST lsp = sreq->hdrs;

    do { 
      fputs(lsp->header, sockfp);
      fputs(CRLF, sockfp);

      if(!vcmd->brief)
        fprintf(stdout, "%s" CRLF, lsp->header);

      if(vcmd->output) {
        fputs(lsp->header, vcmd->output);
        fputs(CRLF, vcmd->output);
      }

      lsp = lsp->next;
    } while(lsp);
  }

  fputs(CRLF, sockfp);

  if(!vcmd->brief)
    fputs(CRLF, stdout);

  if(vcmd->output)
    fputs(CRLF, vcmd->output);

  return sockfp;
}
