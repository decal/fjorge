#include"fjorge.h"
 
signed char **make_hostnames(char **tdoms, const char *restrict *const hosts, size_t hsize) {
	register size_t k = 0, len = hsize, dcnt = 0, i = 0;
  static signed char **aret = NULL;

  /* Due to the conditional return statement below, this function will always *
   * return the value it computed during its first invocation for the entirety *
   * of the program runtime (might be changed in the future, but useful now!) */
  if(aret)
    return aret;

  if(!(hsize && tdoms && hosts))
    return NULL;

  dcnt = array_length(tdoms);

  if(!dcnt)
    return NULL;

  aret = calloc(++len, sizeof *aret);

  if(!aret) {
    perror("calloc");

    return NULL;
  }

  for(len -= 2;k < dcnt; k++) {
    register size_t alen = strlen(tdoms[k]);

    for(i = 0, alen++; i < hsize; ++i, alen++)
      alen += strlen(hosts[i]);

    aret[k] = malloc(alen);

    if(!aret[k])
      perror("malloc");

    for (i = 0; i < hsize; i++) {
      register char *p = (char *)aret[k];

      if(i == len) {
        strcpy(p, tdoms[k]);

        p += strlen(tdoms[k]);
      } else {
        strcpy(p, hosts[i]);

        p+= strlen(hosts[i]);

        strcpy(p, ".");

        p++;
      }
    }
  }

  aret[dcnt] = NULL;

	return aret;
}
