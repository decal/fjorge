#include"fjorge.h"

void signal_handler(const int sign) {
  const char *signame = "(null)";

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

  fjprintf_error("Caught signal: %s(%d)", signame, sign);

  quick_exit(EXIT_FAILURE);
}
