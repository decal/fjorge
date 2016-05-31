#include"fjorge.h"

static int test_arguments(const int optind, const int ac) {
  if(optind >= ac) {
    fputs("*** Expected argument after options" CRLF, stderr);

    return 1;
  }

  return 0;
}

static void show_version(const char *restrict av0) {
  static int sslver_consts[] = { SSLEAY_VERSION, SSLEAY_CFLAGS, SSLEAY_BUILT_ON, SSLEAY_PLATFORM, SSLEAY_DIR };
  register signed int i = 0;

  fputs(CRLF "=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=" CRLF, stdout);
  fputs("= fj0rge version 1.0 by Derek Callaway <decal {at} sdf (dot) org> =" CRLF, stdout);
  fputs("=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=" CRLF CRLF, stdout);
  fputs("GitHub repository: https://github.com/decal/fj0rge" CRLF, stdout);

#ifdef __STD_C_VERSION__
  fprintf(stdout, "ANSI C Standard Library Version: %d" CRLF, __STD_C_VERSION__);
#endif

#ifdef _POSIX_VERSION
  fprintf(stdout, "POSIX Version: %ld" CRLF, _POSIX_VERSION);
#endif

#ifdef _POSIX2_VERSION
  fprintf(stdout, "POSIX.2 Version: %ld" CRLF, _POSIX2_VERSION);
#endif

  fputs("Crypto Library =>", stdout);

  for(i = 0;i < 5;++i) {
    const char *ver = SSLeay_version(sslver_consts[i]);

    switch(i) { 
      case 0:
        fprintf(stdout, " (Version) %s", ver);

        break;
      case 1:
        fprintf(stdout, " (Compile Flags) %s", ver);

        break;
      case 2:
        fprintf(stdout, " (Built On) %s", ver);

        break;
      case 3:
        fprintf(stdout, " (Platform) %s", ver);

        break;
      case 4:
        fprintf(stdout, " (Directory) %s", ver);

        break;
    }
  }

#ifdef ZLIB_VERSION
  fprintf(stdout, CRLF "ZLib Version: %s" CRLF, ZLIB_VERSION);
#endif

  fputs(CRLF, stdout);
  
  exit(EX_OK);
}

/* static void new_header(char **restrict *restrict heads, const char *restrict aline) {
  size_t acnt = 1;
  register char **pp = NULL;

  if(!heads || !aline || !*aline) {
    fputs("*** Invalid input provided to new_header function!" CRLF, stderr);
    exit(EX_DATAERR);
  } else {
    pp = *heads;
    acnt += array_length(pp);
  }

  *heads = calloc(acnt, sizeof **heads);

  if(!*heads)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));
  
  for(pp = *heads;*pp;++pp) {
    *pp = strdup(aline);

    if(!*pp)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));
  }

  *pp = NULL;

  return;
} */

 void parse_cmdline(const int ac, const char **av) {
  int opt = 0;
  char *colon = NULL;

  vcmd = calloc(1, sizeof*vcmd);

  if(!vcmd)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  while ((opt = getopt(ac, (char*const*)av, "bsdvB:Vh:o:")) != -1) {
    switch (opt) {
      case 'b':
        vcmd->brief++;

        break;
      case 's':
        vcmd->secure++;

        break;
      case 'd':
        vcmd->debug++;

        break;
      case 'v':
        vcmd->verbose++;

        break;
      case 'V':
        show_version(*av);

        break;
      case 'B':
        vcmd->basic = strdup(optarg);

        if(!vcmd->basic)
          error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

        basic_auth(vcmd->basic);

        break;
      case 'o':
        vcmd->nameout = strdup(optarg);

        if(!vcmd->nameout)
          error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

        if(!access(vcmd->nameout, F_OK))
          fprintf(stderr, "*** There is a pre-existing file at the desired output path %s (appending to it)" CRLF, vcmd->nameout);

        vcmd->output = fopen(optarg, "a");

        if(!vcmd->output)
          error(1, errno, "fopen: %s", strerror(errno));

        break;
      case 'h':
        add_header(optarg);

        break;
      case 'C': /* CIDR block scan */
      case 'M': /* method scan */
      case 'P': /* port scan */
      case '?':
      default: 
        usage_desc(*av);
    }
  }

  if(test_arguments(optind, ac)) 
    usage_desc(*av);

  vcmd->hostnam = strdup(av[optind]);

  if(!vcmd->hostnam)
    error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

  colon = strchr(vcmd->hostnam, ':');

  if(!colon) {
    vcmd->portnum = 80u;

    if(vcmd->debug)
      fputs("*-* Port number not provided in HOST[:PORT] syntax (defaulting to tcp/80)" CRLF, stderr);
  } else {
    *colon = '\0';

    vcmd->portnum = (unsigned short)strtoul(++colon, NULL, 0x0A);

    if(errno == ERANGE) {
      perror("strtoul");
      usage_desc(*av);
    }
  }

  if(vcmd->secure) {
    if(vcmd->portnum == 80u)
      fputs("*~* Attempting TLS handshake on TCP port 80..is this intentional?" CRLF, stderr);
  } else {
    if(vcmd->portnum == 443u)
      fputs("*~* Attempting TCP connection without TLS on TCP port 443..is this intentional?" CRLF, stderr);
  }

  HTTP_REQUEST *htrequ = &(vcmd->request);

  if(test_arguments(++optind, ac))
    usage_desc(*av);

  htrequ->verb = strdup(av[optind]);

  if(!htrequ->verb)
    error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

  if(test_arguments(++optind, ac))
    usage_desc(*av);

  htrequ->path = strdup(av[optind]);

  if(!htrequ->path)
    error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

  if(test_arguments(++optind, ac))
    usage_desc(*av);

  htrequ->vers = strdup(av[optind]);

  if(!htrequ->vers)
    error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

  PROTOCOL_VERSION *prover = unpack_protover(av[optind]);

  memcpy(&(vcmd->protocol), prover, sizeof*prover);

  free(prover);

  if(av[++optind]) {
    htrequ->host = strdup(av[optind]);

    if(!htrequ->host)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));
  }

  return;
}
