#include"fjorge.h"

_Noreturn void usage_desc(const char *const restrict arg0) {
  puts("");
  printf("usage: %s HOST[:PORT] VERB PATH VERS [HHST[:HPRT]] [-s] [-d] [-v] [-H 'NAME: VALU'] [-o OFIL]\n", arg0);
  printf("\tHOST    numeric IP address or DNS hostname of web server target" CRLF);
  printf("\tPORT    TCP port number to connect to at destination server" CRLF);
  printf("\tVERB    HTTP protocol method: GET, HEAD, POST, OPTIONS, CONNECT" CRLF);
  printf("\tPATH    relative path string or fully qualified URL" CRLF);
  printf("\tVERS    protocol version such as: HTTP/1.0, HTTP/2.0" CRLF);
  printf("\tHHST    the hostname part of a custom Host header" CRLF);
  printf("\tHPRT    the port number part of a custom Host header" CRLF);
  printf("\tNAME    newly created HTTP request header field name" CRLF);
  printf("\tVALU    string value paired up with the new header name" CRLF);
  printf("\tOFIL    path name of output file to write HTTP traffic to" CRLF);
  puts("");
  printf("\t  -s    enable transport stream security with HTTPS via GnuTLS" CRLF);
  printf("\t  -d    debug level which may be used more than once" CRLF);
  printf("\t  -v    verbosity level which may be used more than once" CRLF);
  printf("\t  -o    output the request and response text to a file" CRLF);
  printf("\t  -H    create an HTTP request header, duplicates permitted" CRLF);
  printf("\t-h, -?  display the command line usage info being shown now" CRLF);
  puts("");
  printf("ex. %s www.google.com:80 GET /apps HTTP/1.1 localhost:80" CRLF, arg0);
  printf("ex. %s office365.com:443 HEAD / HTTP/1.0 -s" CRLF, arg0);
  puts("");

  exit(EX_USAGE);
}
