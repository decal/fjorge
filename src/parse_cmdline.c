#include"fjorge.h"

static int test_arguments(const int optind, const int ac) {
  if(optind >= ac) {
    if(optind == 1 && ac == 1)
      fjputs_error("Expected arguments after command name");
    else
      fjputs_error("Expected argument after options");

    return 1;
  }

  return 0;
}

static noreturn void show_version(const char *restrict av0) {
  puts("");

  puts(INVON "=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=" RESET);
  puts(INVON "=" INVOFF " fjorge version 1.0 by Derek Callaway <decal {at} sdf (dot) org> " INVON "=" RESET);
  puts(INVON "=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=*=%=" RESET);

  puts("");

  puts("GitHub repository: https://github.com/decal/fjorge");

  puts("");

#ifdef __STD_C_VERSION__
  printf("ANSI C Standard Library Version: %d ", __STD_C_VERSION__);
#endif

#ifdef _POSIX_VERSION
  printf("POSIX Version: %ld ", _POSIX_VERSION);
#endif

#ifdef ZLIB_VERSION
  printf("ZLib Version: %s ", ZLIB_VERSION);
#endif

#ifdef OPENSSL_VERSION_NUMBER
  printf("OpenSSL Version Number: %lx ", OPENSSL_VERSION_NUMBER);
#endif

#ifdef OPENSSL_THREADS
  puts("OpenSSL Threads Enabled!");
#else
  puts("OpenSSL Threads Disabled!");
#endif

  puts("");

  exit(EX_OK);
}

void parse_cmdline(const int ac, const char **av) {
  int opt = 0;
  char *colon = NULL;

  vcmd = calloc(1, sizeof *vcmd);

  if(!vcmd)
    exit_verbose("calloc", __FILE__, __LINE__);

  cbak = calloc(1, sizeof *cbak);

  if(!cbak)
    exit_verbose("calloc", __FILE__, __LINE__);

  // cbak->set_cookie = 

  while((opt = getopt(ac, (char *const *)av, ":a:bc:de:fh:n:o:t:svyD::E:F::P:V?")) != -1) {
    switch (opt) {
      case 'b':
        vcmd->brief++;

        break;
      case 'c':
        vcmd->cipher = strdup(optarg);

        if(!vcmd->cipher)
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      case 'f':
#ifdef OPENSSL_FIPS
        vcmd->fips++;

        fjprintf_debug("FIPS: %d\n", FIPS_mode());

        if(!FIPS_mode_set(1))
          exit_verbose("FIPS_mode_set", __FILE__, __LINE__);
#endif

        break;
      case 's':
        vcmd->secure++;

        break;
      case 'w':
        vcmd->writebuf = strdup(optarg);

        if(!vcmd->writebuf)
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      case 'd':
        vcmd->callback++; /** TODO: make separate CLI flag for callback? **/
        vcmd->debug++;

        break;
      case 'e':
        vcmd->encode = strdup(optarg);

        if(!vcmd->encode)
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      case 'v':
        vcmd->verbose++;

        break;
      case 'y':
        vcmd->verify++;

        break;
      case 'F':
        vcmd->fuzz++;

        break;
      case 'I': /* intranet scan */
        vcmd->intranet = strdup(optarg);

        if(!vcmd->intranet)
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      case 'V':
        show_version(*av);

        break;
      case 'a':
        vcmd->basic = strdup(optarg);

        if(!vcmd->basic)
          exit_verbose("strdup", __FILE__, __LINE__);

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
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      case 'o':
        vcmd->nameout = strdup(optarg);

        if(!vcmd->nameout)
          exit_verbose("strdup", __FILE__, __LINE__);

        if(!access(vcmd->nameout, F_OK))
          fjprintf_error("There is a pre-existing file at the desired output path %s (appending to it)", vcmd->nameout);
          
        vcmd->output = fopen(optarg, "a");

        if(!vcmd->output)
          error(1, errno, "fopen: %s", strerror(errno));

        break;
      case 'p': /* Port number for Host header and/or SNI */
        break;
      case 't': /* TLS Protocol Number */
        vcmd->protocol = (unsigned int)strtoul(optarg, NULL, 0x0A);

        if(errno == ERANGE)
          describe_usage(*av);

        switch(vcmd->protocol) {
          case 1: /* TLSv1 */
          case 11: /* TLSv1.1 */
          case 12: /* TLSv1.2 */
          case 13: /* TLSv1.3 */
          case 3: /* SSLv3 */
            break;
          default:
            fjputs_error("Incorrect TLS protocol version number supplied!");

            exit(EX_USAGE);
        }

        break;
      case 'h':
        add_header(optarg);

        break;
      case 'A': /* attack */
        vcmd->attack = strdup(optarg);

        if(!vcmd->attack)
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      case 'C': /* CIDR block scan */
      case 'D': /* duplicate headers */
        dup_headers(optarg);

        break;
      case 'E': /* don't encode these characters */
        vcmd->noencode = strdup(optarg);

        if(!vcmd->noencode)
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      case 'M': /* method scan */
      case 'P': /* port scan */
        vcmd->portscan = strdup(optarg);

        if(!vcmd->portscan)
          exit_verbose("strdup", __FILE__, __LINE__);

        break;
      default:
        fjprintf_error("%c is an incorrect command line flag!", (char)opt);
      case '?':
        describe_usage(*av);

        break;
    }
  }

  if(test_arguments(optind, ac)) 
    describe_usage(*av);
 
  vcmd->hostnam = strdup(av[optind]);

  if(!vcmd->hostnam)
    exit_verbose("strdup", __FILE__, __LINE__);

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

      describe_usage(*av);
    }

    if(vcmd->portnum == 443u)
      vcmd->secure++;

    vcmd->portstr = malloc(6);

    if(!vcmd->portstr)
      exit_verbose("malloc", __FILE__, __LINE__);

    sprintf(vcmd->portstr, "%u", vcmd->portnum);
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
    vcmd->acverbv = optind;
    vcmd->verbpav = av[optind];
    htrequ->verb = strdup(av[optind]);

    if(!htrequ->verb)
      exit_verbose("strdup", __FILE__, __LINE__);

    if(++optind == ac) {
      htrequ->path = "/";
      htrequ->vers = "HTTP/1.1";
    } else {
      vcmd->acpathv = optind;
      vcmd->pathpav = av[optind];
      htrequ->path = strdup(av[optind]);

      if(!htrequ->path)
        exit_verbose("strdup", __FILE__, __LINE__);

      if(++optind == ac) {
        htrequ->vers = "HTTP/1.1";
      } else {
        if(test_arguments(optind, ac))
          describe_usage(*av);

        htrequ->vers = strdup(av[optind]);

        if(!htrequ->vers)
          exit_verbose("strdup", __FILE__, __LINE__);

        HTTP_VERSION *const prover = unpack_protover(htrequ->vers);

        memcpy(&(vcmd->version), prover, sizeof *prover);

        // free(prover);

        if(av[++optind] && optind != ac) { /* HTTP Version */
          vcmd->achostv = optind;
          vcmd->hostpav = av[optind];
          htrequ->host = strdup(av[optind]);

          if(!htrequ->host)
            exit_verbose("strdup", __FILE__, __LINE__);

          if(av[++optind] && optind != ac) { /* Max-Forwards */
            vcmd->acmxfwv = optind;
            vcmd->mxfwpav = av[optind];
            htrequ->mxfw = strdup(av[optind]);

            if(!htrequ->mxfw)
              exit_verbose("strdup", __FILE__, __LINE__);
          }
        }
      }
    }
  }

  return;
}
