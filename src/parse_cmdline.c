#include"fjorge.h"

static int test_arguments(const int optind, const int ac) {
  if(optind >= ac) {
    fjputs_error("Expected argument after options");

    return 1;
  }

  return 0;
}

static void show_version(const char *restrict av0) {
  static int sslver_consts[] = { SSLEAY_VERSION, SSLEAY_CFLAGS, SSLEAY_BUILT_ON, SSLEAY_PLATFORM, SSLEAY_DIR };
  register signed int i = 0;

  puts("\n=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=");
  puts("= fjorge version 1.0 by Derek Callaway <decal {at} sdf (dot) org> =");
  puts("=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=\n");
  puts("GitHub repository: https://github.com/decal/fjorge");
  puts("");

#ifdef __STD_C_VERSION__
  printf("ANSI C Standard Library Version: %d" CRLF CRLF, __STD_C_VERSION__);
#endif

#ifdef _POSIX_VERSION
  printf("POSIX Version: %ld" CRLF CRLF, _POSIX_VERSION);
#endif

#ifdef _POSIX2_VERSION
  printf("POSIX.2 Version: %ld" CRLF CRLF, _POSIX2_VERSION);
#endif

  fputs("Crypto Library =>", stdout);

  for(i = 0;i < 5;++i) {
    const char *ver = OpenSSL_version(sslver_consts[i]);

    switch(i) { 
      case 0:
        printf(" (Version) %s", ver);

        break;
      case 1:
        printf(" (Compile Flags) %s", ver);

        break;
      case 2:
        printf(" (Built On) %s", ver);

        break;
      case 3:
        printf(" (Platform) %s", ver);

        break;
      case 4:
        printf(" (Directory) %s", ver);

        break;
    }
  }

  puts("");

#ifdef ZLIB_VERSION
  printf(CRLF "ZLib Version: %s" CRLF CRLF, ZLIB_VERSION);
#endif

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

  vcmd = calloc(1, sizeof *vcmd);

  if(!vcmd)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  while((opt = getopt(ac, (char *const *)av, "a:bc:dfh:o:n:svyzD::V?")) != -1) {
    switch (opt) {
      case 'b':
        vcmd->brief++;

        break;
      case 'c':
        vcmd->cipher = strdup(optarg);

        if(!vcmd->cipher)
          error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

        break;
      case 'f':
#ifdef OPENSSL_FIPS
        vcmd->fips++;

        printf("FIPS: %d\n", FIPS_mode());

        if(!FIPS_mode_set(1))
          error_at_line(1, errno, __FILE__, __LINE__, "FIPS_mode_set: %s", strerror(errno));
#endif

        break;
      case 's':
        vcmd->secure++;

        break;
      case 'd':
        vcmd->callback++; /** TODO: make separate CLI flag for callback? **/
        vcmd->debug++;

        break;
      case 'v':
        vcmd->verbose++;

        break;
      case 'y':
        vcmd->verify++;

        break;
      case 'z':
        vcmd->fuzz++;

        break;
      case 'V':
        show_version(*av);

        break;
      case 'a':
        vcmd->basic = strdup(optarg);

        if(!vcmd->basic)
          error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

        if(!strchr(vcmd->basic, ':')) {
          fjputs_error("Basic authentication string is missing colon!");

          exit(EX_USAGE);
        }

        const char *encbas = encode_base64(vcmd->basic);

        auth_basic(encbas);

        break;
      case 'n':
        vcmd->servername = strdup(optarg);

        if(!vcmd->servername)
          error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

        break;
      case 'o':
        vcmd->nameout = strdup(optarg);

        if(!vcmd->nameout)
          error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

        if(!access(vcmd->nameout, F_OK))
          fjprintf_error("There is a pre-existing file at the desired output path %s (appending to it)", vcmd->nameout);
          
        vcmd->output = fopen(optarg, "a");

        if(!vcmd->output)
          error(1, errno, "fopen: %s", strerror(errno));

        break;
      case 'h':
        add_header(optarg);

        break;
      case 'C': /* CIDR block scan */
      case 'D': /* duplicate headers */
        dup_headers(optarg);

        break;
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
    if(vcmd->secure) {
      vcmd->portnum = 443u;

      if(vcmd->debug) 
        fjputs_debug("Port number not provided in HOST[:PORT] syntax (tcp/443 is the default)");
    } else {
      vcmd->portnum = 80u;

      if(vcmd->debug)
        fjputs_debug("Port number not provided in HOST[:PORT] syntax (tcp/80 is the default)");
    }
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
      fjputs_debug("Attempting TLS handshake on TCP port 80..is this intentional?");
  } else {
    if(vcmd->portnum == 443u)
      vcmd->secure++;
      // fjputs_debug("Attempting TCP connection without TLS on TCP port 443..is this intentional?");
  }

  HTTP_REQUEST *htrequ = &(vcmd->request);

  if(++optind == ac) {
    htrequ->verb = "GET";
    htrequ->path = "/";
    htrequ->vers = "HTTP/1.1";
  } else {
    htrequ->verb = strdup(av[optind]);

    if(!htrequ->verb)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

    if(++optind == ac) {
      htrequ->path = "/";
      htrequ->vers = "HTTP/1.1";
    } else {
      htrequ->path = strdup(av[optind]);

      if(!htrequ->path)
        error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

      if(++optind == ac) {
        htrequ->vers = "HTTP/1.1";
      } else {
        if(test_arguments(optind, ac))
          usage_desc(*av);

        htrequ->vers = strdup(av[optind]);

        if(!htrequ->vers)
          error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

        PROTOCOL_VERSION *prover = unpack_protover(av[optind]);

        memcpy(&(vcmd->protocol), prover, sizeof *prover);

        free(prover);

        if(av[++optind]) {
          htrequ->host = strdup(av[optind]);

          if(!htrequ->host)
            error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));
        }
      }
    }
  }

  return;
}
