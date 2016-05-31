#include"fjorge.h"

char *url_decode(const char *input) {
	int input_length = strlen(input);
	size_t output_length = input_length;
	char *working = NULL, *output = NULL;

  if(!input_length)
    return "";

  working = malloc(++output_length);

  if(!working)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  output = working;
	
	do {
    if(*input == '%') {
			const char buffer[3] = { input[1], input[2], 0 };

			*working++ = strtol(buffer, NULL, 16);
			input += 3;

      continue;
		}
	
    *working++ = *input++;
	} while(*input);

	*working = '\0'; //null terminate

	return output;
}

#ifdef TEST_DRIVE
int main(int argc, char **argv) {
  if(argc > 1) {
    char *input = argv[1];
    char *decoded = url_decode(input);

    fputs(decoded, stdout);
    free(decoded);
  } else {
    char *line = NULL;
    size_t size;

    while(getline(&line, &size, stdin) != -1) {
      char *decoded = url_decode(line);

      fputs(decoded, stdout);
      free(decoded);
    }
  }

  putchar('\n');

  return 0;
}
#endif