#include"fjorge.h"

_Noreturn void usage_desc(const char *const restrict arg0) {
  fprintf(stdout, CRLF "usage: %s HOST[:PORT] VERB PATH VERS [HHST[:HPRT]] [-s] [-d] [-v] [-H 'NAME: VALU'] [-o OFIL]" CRLF, arg0);
  fputs(" HOST  numeric IP address or DNS hostname of web server target" CRLF, stdout);
  fputs(" PORT  TCP port number to connect to at destination server" CRLF, stdout);
  fputs(" VERB  HTTP protocol method: GET, HEAD, POST, OPTIONS, CONNECT" CRLF, stdout);
  fputs(" PATH  relative path string or fully qualified URL" CRLF, stdout);
  fputs(" VERS  protocol version such as: HTTP/1.0, HTTP/2.0" CRLF, stdout);
  fputs(" HHST  the hostname part of a custom Host header" CRLF, stdout);
  fputs(" HPRT  the port number part of a custom Host header" CRLF, stdout);
  fputs(" NAME  newly created HTTP request header field name" CRLF, stdout);
  fputs(" VALU  string value paired up with the new header name" CRLF, stdout);
  fputs(" OFIL  path name of output file to write HTTP traffic to" CRLF, stdout);
  fputs(CRLF, stdout); 
  fputs(" -?  display the command line usage info being shown now" CRLF, stdout);
  fputs(" -d  debug level (may be used more than once)" CRLF, stdout);
  fputs(" -o  output the request and response text to a file" CRLF, stdout);
  fputs(" -s  enable transport stream security with HTTPS via GnuTLS" CRLF, stdout);
  fputs(" -v  verbosity level (may be used more than once)" CRLF, stdout);
  fputs(" -h  create an HTTP request header, duplicates permitted" CRLF, stdout);
  fputs(CRLF, stdout);
  fprintf(stdout, "ex. %s www.google.com:80 GET /apps HTTP/1.1 localhost:80" CRLF, arg0);
  fprintf(stdout, "ex. %s office365.com:443 HEAD / HTTP/1.0 -s" CRLF CRLF, arg0);

  exit(EX_USAGE);
}
