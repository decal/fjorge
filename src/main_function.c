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

  parse_cmdline(argc, (const char **)argv);

  if(vcmd->debug)
    print_options(stderr);

  if(vcmd->secure) {
    atls = connect_tls(vcmd->hostnam, vcmd->portnum);

    fjprintf_verbose("TLS Connection to %s:%hu", vcmd->hostnam, vcmd->portnum);
  } else {
    asfd = connect_tcp(vcmd->hostnam, vcmd->portnum);

    fjprintf_verbose("TCP Connection to %s:%hu", vcmd->hostnam, vcmd->portnum);

    if(asfd < 0) {
      fjprintf_error("Unable to connect to %s:%hu", vcmd->hostnam, vcmd->portnum); 

      goto _fin;
    }
  }

  if(vcmd->secure) {
    register unsigned int k = 0;

    /** TODO: pipelining/persistent connections **/
    /* for(k = 0;k < 2;k++)  */
      if(send_tls(atls, &(vcmd->request))) {
        do {
          rlen = recv_tls(atls);

          switch(rlen) {
            case 0:
            case -1:
              break;
            case -2:
              error_tls(NULL, rlen, "BIO_read");

              break;
            default:
              fjprintf_debug("Received %lu bytes via encrypted TLS connection..", rlen);

              break;
          }

          break;
        } while(BIO_should_retry(atls));
      }
  } else {
    register unsigned int x = 0;

    /* for(x = 0;x < 2;x++) {  */
      anfp = send_request(asfd, &(vcmd->request));

      if(!anfp) {
        fjputs_error("Unable to send plaintext request!");

        exit(EX_IOERR);
      }

      rlen = recv_response(anfp);

      fjprintf_debug("Received %lu bytes via plaintext TCP connection..", rlen);
    /* } */
  }

_fin:
  if(error_message_count > 0) /* make this an optional atexit_handler #ifdef DEBUG */
    fjprintf_debug("*** %u error messages!", error_message_count);

  exit(EXIT_SUCCESS);
}
