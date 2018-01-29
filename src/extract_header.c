#include"fjorge.h"

/*
 * Extract HTTP response header name and value from received network buffer.
 * @pre line must not start or end with whitespace 
 * @post 
 * @param line field-content portion of BNF from "HTTP/1.1 Message Syntax and Routing" (RFC7230)
 * @return data structure containing a tokenized header field
 * @see https://tools.ietf.org/html/rfc7230#section-3.2
 */
HEADER_LINE *extract_header(char *restrict line) {
  assert(line);

  HEADER_LINE *hln = calloc(1, sizeof *hln);

  if(!hln)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  char *restrict cln = strchr(line, ':');

  if(!cln) {
    fjputs_error("Colon delimiter missing from HTTP response header!");
    fjputs_error(line);

    hln->name = strdup(line);

    if(!hln->name)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

    return NULL;
  }

  *cln++ = '\0';

  hln->name = strdup(line);

  if(!hln->name)
    error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

  for(;cln && *cln;++cln)
    if(!isspace(*cln))
      break;

  hln->value = strdup(cln);

  if(!hln->value) 
    error_at_line(1, errno,  __FILE__, __LINE__, "strdup: %s", strerror(errno));

  return hln;
}
