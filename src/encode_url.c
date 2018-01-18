#include"fjorge.h"

static int is_non_symbol(const char c) {
  if(!c) 
    return 1; /* we want to write null regardless */

  const int c_int = (int) c;

  return (c_int >= 48 && c_int <= 57) || (c_int >= 65 && c_int <= 90) || (c_int >= 97 && c_int <= 122);
}

char *encode_url(const char *input) {
  size_t end = strlen(input), final_size = end * 3;
  char *working = NULL, *output = NULL;

  if(!end)
    return "";

  working = malloc(++final_size);

  if(!working)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  output = working;

  do {
    const char c = *input;

    if(c < 0) {
      input++;
    } else if(is_non_symbol(c)) {
      *working++ = *input++;
    } else {
      char encoded[4] = { 0x00 };

      snprintf(encoded, 4, "%%%02x", c);

      *working++ = encoded[0];
      *working++ = encoded[1];
      *working++ = encoded[2];

      input++;
    }
  } while(*input);

  *working = '\0'; //null term

  return output;
}

#ifdef TEST_DRIVE
int main(int argc, char **argv) 
{
  if(argc > 1) {
    char *input = argv[1];
    const char *encoded = encode_url(input);

    puts(encoded);
    free(encoded);
  } else {
    char *line = NULL;
    size_t size = 0;

    while(getline(&line, &size, stdin) != -1) {
      const char *encoded = encode_url(line);

      puts(encoded);
      free(encoded);
    }
  }

  return 0;
}
#endif
