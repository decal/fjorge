#include"fjorge.h"

static void array_puts(char **arr) {
  register char **pp = arr;

  while(pp && *pp)
    puts(*pp++);

  return;
}

static char **argv_copy(char **);

/* COMMAND_LINE cmdl; */
COMMAND_LINE *vcmd = NULL;
OUTPUT_VALUE *outv = NULL;
CALLBACK_FUNCPTRS *cbak = NULL;
BIO *bioout = NULL;
BIO *bioerr = NULL;
BIO *bioin = NULL;
char *ahost = NULL;
char *bhost = NULL;
char *chost = NULL;

// int main(int argc, char *argv[], char *envp[]) {
int main(int argc, char *argv[]) {
  register signed int asfd = 0;
  size_t rlen = 0;
  FILE *anfp = NULL;
  BIO *atls = NULL;
  int xargc = argc;
  char **xargv = argv_copy(argv);

  signal(SIGSEGV, signal_handler);
  signal(SIGINT,  signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGILL,  signal_handler);
  signal(SIGPIPE, signal_handler);

  parse_cmdline(argc, (const char **)argv);

  if(vcmd->debug)
    print_options(stderr);

  bioout = BIO_new_fp(stdout, BIO_NOCLOSE);

  if(!bioout) {
    fjputs_error("Unable to create a BIO for stdout!");

    exit(EX_SOFTWARE);
  }

  bioerr = BIO_new_fp(stderr, BIO_NOCLOSE);

  if(!bioerr) {
    fjputs_error("Unable to create a BIO for stderr!");

    exit(EX_SOFTWARE);
  }

  bioin = BIO_new_fp(stdin, BIO_NOCLOSE);

  if(!bioin) {
    fjputs_error("Unable to create a BIO for stdin!");

    exit(EX_SOFTWARE);
  }

_top:
  fjputs_debug("Starting!");

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

    if(!atls)
      goto _fin;

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
  fjprintf_debug("Finished!");

  HTTP_REQUEST *hreq = &(vcmd->request);

  if(hreq->prts) {
    unsigned short *prtz = array_portlist(hreq->prts);
    unsigned short *uspp = prtz;
    size_t hpsz = 7 + strlen(hreq->host);
    char *ohst = strdup(hreq->host);

    if(!ohst)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

    char *ahst = malloc(hpsz);

    if(!ahst)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

    while(uspp && *uspp) {
      sprintf(ahst, "%s:%d", ahost, *uspp);

      hreq->host = strdup(ahst);

      if(!hreq->host)
        error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

      xargv[5] = strdup(hreq->host);

      if(!xargv[5])
       error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

      main(xargc, xargv);

      uspp++;
    }
  }

  if(error_message_count > 0) /* make this an optional atexit_handler #ifdef DEBUG */
    fjprintf_debug("*** %u error messages!", error_message_count);

  puts("");

  return 0;
}

static size_t argv_len(char **av) {
  register char **pp = av;
  size_t l = 0;

  while(pp && *pp) {
    l++;
    pp++;
  }

  return l;
}

static char **argv_copy(char **argv) {
  char **pp = argv;
  size_t len = 0;
  char **rr = NULL;

  assert(argv);

  len += argv_len(argv);

  rr = malloc(++len * sizeof *rr);

  if(!rr)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  int i = 0;

  while(pp && *pp) {
    rr[i] = strdup(*pp);

    if(!rr[i])
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

    pp++;
    i++;
  }

  rr[i] = NULL;

  return rr;
}
