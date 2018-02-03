#include"fjorge.h"

//static int is_non_symbol(const char c) {
//  if(!c) 
//    return 1; 

//  const int c_int = (int) c;
  
  /* 48='0' 57='9' */ /* 65='A' 90='Z' */ /* 97='a' 122='z' */
//  return (c_int >= 48 && c_int <= 57) || (c_int >= 65 && c_int <= 90) || (c_int >= 97 && c_int <= 122);
//}

typedef enum urlencode_flags {
  ENCINCL = 0x2,
  ENCEXCL = 0x4,
  ENCRAND = 0x8
} URLENCODE_FLAGS;

static unsigned int istrue_random(void) {
  return rand() % 2;
}

/*
 * Unreserved characters according to RFC3986 Section 2.3
 * 
 * @see https://tools.ietf.org/html/rfc3986#section-2.3
 *
 */
static int isunres(const int c, unsigned int r) {
  if(r)
    return rand() % 2;

  if(isalnum(c))
    return 1;

  switch(c) {
    case '-':
    case '.':
    case '_':
    case '~':
      return 1;
  }

  return 0;
}

static char *restrict urlencode_char(const int c, char *restrict working) {
  char encoded[4] = { 0x00 };

  snprintf(encoded, 4, "%%%02x", c);

  *working++ = encoded[0];
  *working++ = encoded[1];
  *working++ = encoded[2];

  return working;
}

static char *restrict urlencode_char_random(const int c, char *restrict working) {
  if(istrue_random())
    *working++ = c;
  else
    working = urlencode_char(c, working);

  return working;
}

static void encode_url_helper(char *restrict working, const char *restrict input, const char *const charz, const unsigned int flagz) {
  do {
    const int c = (int)*input;
    char *x = NULL;
    
    if(charz)
      x = strchr(charz, c);

    if(x) { /* include, exclude or random.. can't have multiple set! */
      if((flagz & ENCINCL) == ENCINCL) 
        working = urlencode_char(c, working);
      else if((flagz & ENCEXCL) == ENCEXCL) 
        *working++ = *input;
      else if((flagz & ENCRAND) == ENCRAND)
        working = urlencode_char_random(c, working);
      else
        *working++ = *input;
    } else {
      if((flagz & ENCINCL) == ENCINCL)
        working = urlencode_char(c, working);
      else if((flagz & ENCEXCL) == ENCEXCL) 
        *working++ = *input;
      else if((flagz & ENCRAND) == ENCRAND) {
        if(isunres(c, 1))
          *working++ = *input;
        else
          working = urlencode_char_random(c, working);
      } else {
        if(isunres(c, 0))
          *working++ = *input;
        else
          working = urlencode_char(c, working);
      }
    } 
  } while(*++input);

  *working = '\0';

  return;
}

char *encode_url(const char *restrict input, const char *const charz, const unsigned int flagz) {
  assert(input);

  const size_t iend = strlen(input);
  size_t wsiz = iend * 3;
  char *restrict working = NULL;

  if(!iend)
    return "";

  working = malloc(++wsiz);

  if(!working)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  encode_url_helper(working, input, charz, flagz);

  return working;
}

#ifdef TEST_DRIVE
int main(int argc, char **argv) 
{
  char chars[] = "abc123xyz456";

  srand(time(NULL));

  if(argc > 1) {
    char *input = argv[1];
    // const char *encoded = encode_url(input, chars, ENCINCL);
    const char *encoded = encode_url(input, chars, ENCINCL);

    puts(encoded);

    encoded = encode_url(input, chars, ENCEXCL);

    puts(encoded);

    encoded = encode_url(input, chars, ENCRAND);

    puts(encoded);

    encoded = encode_url(input, chars, 0);

    puts(encoded);

    encoded = encode_url(input, 0, ENCINCL);

    puts(encoded);

    encoded = encode_url(input, 0, ENCEXCL);

    puts(encoded);

    encoded = encode_url(input, 0, ENCRAND);

    puts(encoded);

    encoded = encode_url(input, 0, 0);

    puts(encoded);
  } else {
    char *line = NULL;
    size_t size = 0;

    while(getline(&line, &size, stdin) != -1) {
      line[strlen(line) - 1] = '\0';

      char *encoded = encode_url(line, chars, ENCINCL);

      puts(encoded);
      free(encoded);
    }
  }

  return 0;
}
#endif
