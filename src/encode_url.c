#include"fjorge.h"

//static int is_non_symbol(const char c) {
//  if(!c) 
//    return 1; 

//  const int c_int = (int) c;
  
  /* 48='0' 57='9' */ /* 65='A' 90='Z' */ /* 97='a' 122='z' */
//  return (c_int >= 48 && c_int <= 57) || (c_int >= 65 && c_int <= 90) || (c_int >= 97 && c_int <= 122);
//}

#define ENCINCL 0x1
#define ENCEXCL 0x2
#define ENCRAND 0x4
#define ENCURES 0x8
#define ENCALLC 0x10
#define ENCNONE 0x20

/*
 * Unreserved characters according to RFC3986 Section 2.3.
 * @param c character to test for being an element of the unreserved character set
 * @return int non-zero if c falls within the unreserved character set, zero otherwise
 * @see https://tools.ietf.org/html/rfc3986#section-2.3
 */
static int isunres(const int c) {
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

static void encode_url_normal(char *restrict working, const char *restrict input, const int flagz) {
  char encoded[4] = { 0x00 };

  do {
    const int c = (int)*input;

    if((flagz & ENCALLC) == ENCALLC) {
      snprintf(encoded, 4, "%%%02x", c);
      strncpy(working, encoded, 3);

      working += 3;

      continue;
    } else if((flagz & ENCNONE) == ENCNONE) {
      *working++ = *input;

      continue;
    }

    if(isunres(c)) {
      if((flagz & ENCURES) == ENCURES) { 
        snprintf(encoded, 4, "%%%02x", c);
        strncpy(working, encoded, 3);

        working += 3;
      } else {
        *working++ = *input;
      }
    } else {
      snprintf(encoded, 4, "%%%02x", c);
      strncpy(working, encoded, 3);

      working += 3;
    }
  } while(*input++);

  *working = '\0';

  return;
}

static char *encode_url_helper(char *restrict working, const char *restrict input, const char *const charz, unsigned int flagz) {
  char *workret = working, encoded[4];

  do {
    const int c = (int)*input;

    if((flagz & ENCINCL) == ENCINCL && !isunres(c)) {
      if(strchr(charz, c)) {
        snprintf(encoded, 4, "%%%02x", c);
        strncpy(working, encoded, 3);

        working += 3;
      }
    } else if((flagz & ENCEXCL) == ENCEXCL && !isunres(c)) {
      if(!strchr(charz, c)) {
        snprintf(encoded, 4, "%%%02x", c);
        strncpy(working, encoded, 3);
      
        working += 3;
      } else if((flagz & ENCURES) == ENCURES && isunres(c)) { 
        snprintf(encoded, 4, "%%%02x", c);
        strncpy(working, encoded, 3);

        working += 3;
      } else {
        *working++ = *input;
      }
    } else {
      *working++ = *input;
    }
  } while(*input++);

  *working = '\0';

  return workret;
}

static char *encode_url_random(char *restrict working, const char *restrict input, const char *const charz, unsigned int flagz) {
  return NULL;
}

char *encode_url(const char *restrict input, const char *const charz, unsigned int flagz) {
  assert(input);

  const size_t iend = strlen(input);
  size_t wsiz = iend * 3, csiz = 0;
  char *restrict working = NULL;

  if(!iend)
    return "";

  working = malloc(++wsiz);

  if(!working)
    error_at_line(1, errno, __FILE__, __LINE__, "malloc: %s", strerror(errno));

  char *output = working;

  if(charz) {
    csiz += strlen(charz);
  } else {
    if(!flagz) {
      encode_url_normal(working, input, flagz);
    } else { 
      if((flagz & ENCINCL) == ENCINCL) {
        encode_url_helper(working, input, charz, flagz);
      } else {
        if((flagz & ENCINCL) == ENCINCL) {
          encode_url_helper(working, input, charz, flagz);  
        } else {
          if((flagz & ENCRAND) == ENCRAND)
            encode_url_random(working, input, charz, flagz);
          else
            encode_url_normal(working, input, flagz);
        }
      }
    }
  }

  return output;
}

#ifdef TEST_DRIVE
int main(int argc, char **argv) 
{
  if(argc > 1) {
    char *input = argv[1];
    const char *encoded = encode_url(input);
  } else {
    char *line = NULL;
    size_t size;

    while(getline(&line, &size, stdin) != -1) {
      line[strlen(line) - 1] = '\0';

      char *encoded = encode_url(line);

      puts(encoded);
      free(encoded);
    }
  }

  return 0;
}
#endif
