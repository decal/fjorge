#include"fjorge.h"

char *decode_url(const char *input) {
  assert(input);

	const size_t input_length = strlen(input);
	size_t output_length = input_length;
	char *working = NULL;

  if(!input_length)
    return "";

  char *output = malloc(++output_length);

  if(!output)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  working = output;
	
  do {
    if(*input == '%') {
      const char buffer[3] = { input[1], input[2], 0 };

      *working++ = strtol(buffer, NULL, 16);

      if(errno == ERANGE)
        error_at_line(1, errno, __FILE__, __LINE__, "strtol: %s", strerror(errno));

      input += 3;

      continue;
    }

    *working++ = *input++;
  } while(*input);

	*working = '\0';

	return output;
}

#ifdef TEST_DRIVE
int main(int argc, char **argv)
{
  if(argc > 1) {
    char *input = argv[1];
    char *decoded = decode_url(input);

    fputs(decoded, stdout);
    free(decoded);
  } else {
    char *line = NULL;
    size_t size;

    while(getline(&line, &size, stdin) != -1) {
      char *decoded = decode_url(line);

      fputs(decoded, stdout);
      free(decoded);
    }
  }

  putchar('\n');

  return 0;
}
#endif
