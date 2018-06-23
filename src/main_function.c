#include"fjorge.h"
#include"strglob.h"

extern char **environ;

static char **argv_copy(char **);
static int argv_len(char **av); /* __attribute__ ((pure)); */

COMMAND_LINE *vcmd = NULL;
/* OUTPUT_VALUE *vout = NULL; */
CALLBACK_FUNCPTRS *cbak = NULL;
BIO *bioout = NULL;
BIO *bioerr = NULL;
BIO *bioin = NULL;
/* char *request_header_host = NULL, *tls_sni_host = NULL, *request_line_host = NULL; */

int main(int argc, char *argv[]) {
  BIO *atls = NULL;
  char **xargv = argv_copy(argv);

  if(getenv("FJORGE_DAEMON")) {
    setsid();

    (int) daemon(true, true);
  }

  signal(SIGSEGV, signal_handler);
  signal(SIGINT,  signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGILL,  signal_handler);
  signal(SIGPIPE, signal_handler);

  parse_cmdline(argc, (const char **)argv);

  if(vcmd->debug) {
    // OPENSSL_malloc_init();

    print_options(stderr);
  }

  bioout = BIO_new_fp(stdout, BIO_NOCLOSE);

  if(!bioout) {
    fjputs_error("Unable to create a BIO for stdout!");

    return EX_SOFTWARE;
  }

  bioerr = BIO_new_fp(stderr, BIO_NOCLOSE);

  if(!bioerr) {
    fjputs_error("Unable to create a BIO for stderr!");

    return EX_SOFTWARE;
  }

  bioin = BIO_new_fp(stdin, BIO_NOCLOSE);

  if(!bioin) {
    fjputs_error("Unable to create a BIO for stdin!");

    return EX_SOFTWARE;
  }

  if(vcmd->secure) {
    atls = connect_tls(vcmd->hostnam, vcmd->portnum);

    fjprintf_verbose("TLS Connection to %s:%hu", vcmd->hostnam, vcmd->portnum);
  } else {
    const signed int asfd = connect_tcp(vcmd->hostnam, vcmd->portnum);

    fjprintf_verbose("TCP Connection to %s:%hu", vcmd->hostnam, vcmd->portnum);

    if(asfd < 0) {
      fjprintf_error("Unable to connect to %s:%hu", vcmd->hostnam, vcmd->portnum); 

      goto _fin;
    }

    atls = create_sockbio(asfd);
  }

  if(!atls)
    goto _fin;

  HTTP_REQUEST *hreq = &(vcmd->request);

  /** TODO: pipelining/persistent connections **/
  if(send_request(atls, hreq)) {
    for(register unsigned int k = 0;k < 10;++k) {
      do {
        const size_t rlen = recv_response(atls, hreq);

        switch(rlen) {
          case 0:
          case -1:
            k = 10;

            continue;
          case -2:
            error_tls(NULL, rlen, "BIO_read");

            break;
          default:
            fjprintf_debug("Received %lu bytes via encrypted TLS connection..", rlen);

            break;
        }
      } while(BIO_should_retry(atls));

      break;
    }
  }

_fin:
  do { } while(false);

  if(!getenv("FJORGE_DAEMON")) {
    HAND_GLOB *host_hagl = handle_strglob(hreq->host);
    char **host_strs = copy_strglob(host_hagl);
    HAND_GLOB *verb_hagl = handle_strglob(hreq->verb);
    char **verb_strs = copy_strglob(verb_hagl);
    HAND_GLOB *path_hagl = handle_strglob(hreq->path);
    char **path_strs = copy_strglob(path_hagl);

    register int host_cnt = 0;

    for(register char **restrict host_pp = host_strs;*host_pp;host_pp++, host_cnt++) {
      char *ahost = strcat_glob(host_pp, host_hagl->size);

      xargv[vcmd->achostv] = ahost;
      vcmd->hostpav = ahost;

      for(register char **restrict verb_pp = verb_strs;*verb_pp;verb_pp++) {
        char *averb = strcat_glob(verb_pp, verb_hagl->size);

        vcmd->verbpav = averb;
        xargv[vcmd->acverbv] = averb;

        for(register char **restrict path_pp = path_strs;*path_pp;path_pp++) {
          char *apath = strcat_glob(path_pp, path_hagl->size); 

          vcmd->pathpav = apath;
          xargv[vcmd->acpathv] = apath;

          if(!(host_cnt % host_hagl->size)) {
            putenv("FJORGE_DAEMON=1");

            main(argc, xargv);

            unsetenv("FJORGE_DAEMON");
          }
        }
      } 
    }
  }

  if(error_message_count > 0) /* make this an optional atexit_handler #ifdef DEBUG */
    fjprintf_debug("*** %u error messages!", error_message_count);

  
  return EXIT_SUCCESS;
}

static int argv_len(char **av) {
  register int l = 0;

  if(av)
    for(register char *const *restrict pp=av;*pp;pp++)
      l++;

  return l;
}

static char **argv_copy(char **argv) {
  assert(argv);

  register size_t len = argv_len(argv);

  char **rr = malloc(++len * sizeof *rr);

  if(!rr)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  register int i = 0;

  for(i = 0;i < len;i++) {
    if(!argv[i])
      break;

    rr[i] = strdup(argv[i]);

    if(!rr[i])
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));
  }

  rr[i] = NULL;

  return rr;
}
