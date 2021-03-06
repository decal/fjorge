#include"fjorge.h"

/* Obtain a backtrace and print it to stdout. */

void print_trace (void) {
  void *array[10] = { 0x0 };
  const size_t size = backtrace(array, 10);
  char **strings = backtrace_symbols(array, size);
  register size_t i = 0;

  fjprintf_debug("Obtained %zd stack frames.", size);

  if(!strings)
    return;

  if(vcmd->brief) {
    fputs(BADGE_TRACE, stderr);

    do {
      fputs(strings[i++], stderr);
      fputc(' ', stderr);
    } while(i < size);

    fputc('\n', stderr);
  } else {
    do {
      fputs(BADGE_TRACE, stderr);
      fputs(strings[i++], stderr);
      fputc('\n', stderr);
    } while(i < size);
  }

  // free(strings);

  return;
}

#if 0
/* A dummy function to make the backtrace more interesting. */
void dummy_function (void) {
  print_trace ();

  return;
}

int main (void) {
  dummy_function ();

  return 0;
}
#endif
