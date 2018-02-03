#include"fjorge.h"

static void array_puts(char **arr) {
  register char **pp = arr;

  while(pp && *pp)
    puts(*pp++);

  return;
}

extern char **environ;

void run_cmd(const char **av) {
  assert(acmd);

  auto pid_t apid = 0;

  fjprintf_debug("Spawning command: %s", *av);

  if(posix_spawn(&apid, *av, NULL, NULL, (char *const *)av, environ)) {
    fjprintf_error("posix_spawn: %s", strerror(errno));

    return;
  }

  fjprintf_debug("Child PID: %i", apid);

  /* if (waitpid(pid, &status, 0) != -1) 
    printf("Child exited with status %i\n", status);
  else
    perror("waitpid"); */

  return;
}

static const char **argv_copy(char **);

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

int main(int argc, char *argv[]) {
  register signed int asfd = 0;
  size_t rlen = 0;
  BIO *atls = NULL;
  const char **xargv = argv_copy(argv);

  if(getenv("FJORGE_DAEMON")) {
    setsid();

    daemon(true, true);
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

  if(!vcmd->secure)
    atls = create_sockbio(asfd, &(vcmd->request));

  if(!atls)
    goto _fin;

  HTTP_REQUEST *hreq = &(vcmd->request);

  /** TODO: pipelining/persistent connections **/
  if(send_request(atls, hreq)) {
    int cont = 1;

    for(register unsigned int k = 0;cont && k < 10;++k)
      do {
        rlen = recv_response(atls);

        switch(rlen) {
          case 0:
          case -1:
            cont = 0;

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

_fin:
  do { } while(0);

  if(hreq->prts) {
    unsigned short *const prtz = array_portlist(hreq->prts);
    unsigned short *uspp = prtz;
    const size_t hpsz = 7 + strlen(hreq->host);
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

      putenv("FJORGE_DAEMON=1");

      run_cmd(xargv);

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

static const char **argv_copy(char **argv) {
  char **pp = argv;
  size_t len = 0;
  const char **rr = NULL;

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
