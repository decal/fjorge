#include"fjorge.h"

/* Obtain a backtrace and print it to stdout. */
void
print_trace (void)
{
  void *array[10];
  size_t size = backtrace (array, 10);
  char **strings = backtrace_symbols (array, size);
  register size_t i = 0;

  printf("Obtained %zd stack frames.\n", size);

  do 
    printf("%s\n", strings[i++]);
  while(i < size);

  free(strings);
}

#if 0
/* A dummy function to make the backtrace more interesting. */
void
dummy_function (void)
{
  print_trace ();
}

int
main (void)
{
  dummy_function ();
  return 0;
}
#endif
