#include"fjorge.h"

/**
 * @note gcc -o rewrite_arg2head rewrite_arg2head.c -DTEST_DRIVE -I../include
 */

#ifdef TEST_DRIVE
void fjputs_error(const char *emsg) {
  fputs(emsg, stderr);
  fputc('\n', stderr);

  return;
}
#endif

char *rewrite_arg2head(char *anarg) {
  assert(anarg);

  const size_t alen = 2 + strlen(anarg); /* extra space, plus null terminator */
  char *rstr = malloc(alen);

  if(!rstr)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  strcpy(rstr, anarg);

  register char *pa = anarg, *pr = rstr;

  *pr++ = toupper(*pa++);

  while((char)*pa) {
    switch((char)*pa) {
      case '-':
        *pr++ = *pa++;

        if(!*pa) {
          fjputs_error("String ends prematurely after dash!");

          exit(EX_DATAERR);
        }

        if(!isalpha(*pa)) {
          fjputs_error("Character after dash is not alphabetic!");

          exit(EX_DATAERR);
        }

        *pr++ = toupper(*pa++);

        break;
      case ':':
        *pr++ = *pa++;

        if(!*pa) {
          fjputs_error("String ends prematurely after colon!");

          exit(EX_DATAERR);
        }

        while(isspace(*pa)) {
          pa++;

          if(!*pa) {
            fjputs_error("String ends prematurely after white space!");

            exit(EX_DATAERR);
          }
        }

        *pr++ = ' ';
        *pr++ = *pa++;

        break;
      default:
        *pr++ = *pa++;
    }
  }

  *pr = '\0';

  return rstr;
}

#ifdef TEST_DRIVE
int main(int argc, char *argv[]) {
  if(argc < 2) {
    fprintf(stderr, "usage: %s HEADER\n", *argv);
    fputs("  HEADER  HTTP header in the form 'name: value'\n", stderr);

    return 1;
  }

  const char *ahed = rewrite_arg2head(argv[1]);

  puts(ahed);

  return 0;
}
#endif
