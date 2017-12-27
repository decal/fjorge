#include"fjorge.h"

void signal_handler(const int sign) {
  const char *restrict signame = NULL;

  print_trace();

  switch(sign) {
    case SIGSEGV:
      signame = "SIGSEGV";

      break;
    case SIGINT:
      signame = "SIGINT";

      break;
    case SIGTERM:
      signame = "SIGTERM";

      break;
    case SIGILL:
      signame = "SIGILL";

      break;
    case SIGPIPE:
      signame = "SIGPIPE";

      break;
    case SIGFPE:
      signame = "SIGFPE";

      break;
    default:
      signame = "UNKNOWN!";
  }

  fprintf(stderr, "*!* Caught signal identifier: %s numeric constant: %d" CRLF, signame, sign);

  quick_exit(EXIT_FAILURE);
}
