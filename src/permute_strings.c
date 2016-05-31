#include"fjorge.h"
 
/* Boothroyd method; exactly N! swaps, about as fast as it gets */
static void boothroyd_method(const char *restrict *const doms, const char *restrict *const x, const unsigned int n, const unsigned int nn, signed char **callback(const char *restrict *const, const char *restrict *const, size_t)) {
	register unsigned int c = 0, i = 0, m = n - 1;

	do {
		if (n > 2) 
      boothroyd_method(doms, x, m, nn, callback);

		if (c >= m) 
      return;
 
		i = (n & 1) ? 0 : c++;

    if(x[m] != x[i]) {
      register const char *restrict t = x[m];

      x[m] = x[i];
      x[i] = t; 
    }

		if (callback) 
      callback(doms, x, nn);
	} while(1);

  return;
}
 
/* entry for Boothroyd method */
void permute_strings(const char *restrict *const d, const char *restrict *const x, const unsigned int n, const unsigned int nn, signed char **callback(const char *restrict *const, const char *restrict *const, size_t)) {
  if(vcmd->debug) {
    if (callback) 
      callback(d, x, n);

    boothroyd_method(d, x, n, nn, callback);
  } else {
    boothroyd_method(d, x, n, nn, NULL);
  }

  return;
}

#if 0

#define N 4

int main() {
	register int i;
  char *domains[] = { "google.com", "gist.github.io" };
  char *x[9];

	for (i = 0; i < N; i++) {
    x[i] = malloc(2);

    if(!x[i])
      perror("malloc");

    x[i][0] = i + '0';
    x[i][1] = '\0';
  }
 
  permute_strings("google.com", x, N, puts_hostnames);

	return 0;
}
#endif
