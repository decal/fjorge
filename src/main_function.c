#include"fjorge.h"

COMMAND_LINE *vcmd = NULL;

int main(int argc, char *argv[], char *envp[]) {
  register signed int asfd = 0;
  size_t rlen = 0;
  FILE *anfp = NULL;
  BIO *atls = NULL;

  signal(SIGSEGV, signal_handler);
  signal(SIGINT,  signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGILL,  signal_handler);
  signal(SIGPIPE, signal_handler);

  vcmd = parse_cmdline(argc, (const char**)argv);

  if(vcmd->debug)
    print_options(stderr);

  if(vcmd->secure) {
    atls = tls_connect(vcmd->hostnam, vcmd->portnum);

    if(vcmd->verbose) 
      fprintf(stderr, "*%%* TLS Connection to %s:%hu" CRLF, vcmd->hostnam, vcmd->portnum);
  } else {
    asfd = tcp_connect(vcmd->hostnam, vcmd->portnum);

    if(vcmd->verbose) 
      fprintf(stderr, "*%%* TCP Connection to %s:%hu" CRLF, vcmd->hostnam, vcmd->portnum);

    if(asfd < 0) {
      fprintf(stderr, "*** Unable to connect to %s:%hu" CRLF, vcmd->hostnam, vcmd->portnum);

      goto _fin;
    }
  }

  if(vcmd->secure) {
    if(tls_send_request(atls, &(vcmd->request))) {
      rlen = tls_recv_response(atls);

      if(vcmd->debug)
        fprintf(stderr, "*** Received %lu bytes via encrypted TLS connection.." CRLF, rlen);
    }
  } else {
    anfp = send_request(asfd, &(vcmd->request));

    if(!anfp) {
      fputs("*** Unable to send plaintext request!" CRLF, stderr);
      exit(EX_IOERR);
    }

    rlen = recv_response(anfp);

    if(vcmd->debug)
      fprintf(stderr, "*** Received %lu bytes via plaintext TCP connection.." CRLF, rlen);
  }

_fin:
  if(error_message_count > 0) /* make this an optional atexit_handler #ifdef DEBUG */
    fprintf(stderr, "*** %u error messages!" CRLF, error_message_count);

  exit(EXIT_SUCCESS);
}
