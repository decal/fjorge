#include"fjorge.h"

noreturn void describe_usage(const char *const restrict arg0) {
  printf(CRLF "usage: %s HOST[:PORT] VERB PATH VERS [HHST[:HPRT]] [-d] [-s] [-v] [-y] [-z] [-n D.OM] [-h 'NAME: VALU'] [-o OFIL] [-B U:PW] [-D [[NAME,DUPS]|[DUPS,NAME]]" CRLF, arg0);
  fputs(" HOST  numeric IP address or DNS hostname of target web server" CRLF, stdout);
  fputs(" PORT  TCP port number to connect to on destination server" CRLF, stdout);
  fputs(" VERB  HTTP protocol method: GET, HEAD, POST, OPTIONS, CONNECT" CRLF, stdout);
  fputs(" PATH  relative path string or fully qualified URL" CRLF, stdout);
  fputs(" VERS  protocol version string such as: HTTP/1.0, HTTP/2.0, etc." CRLF, stdout);
  fputs(" HHST  the hostname part of a custom Host header" CRLF, stdout);
  fputs(" HPRT  the port number part of a custom Host header" CRLF, stdout);
  fputs(" NAME  HTTP request header field name" CRLF, stdout);
  fputs(" VALU  string value paired up with the new header name" CRLF, stdout);
  fputs(" OFIL  path name of output file to write HTTP traffic to" CRLF, stdout);
  fputs(" DUPS  number of duplicate HTTP request headers to create" CRLF, stdout);
  fputs(" D.OM  hostname for SNI (Server Name Indication)" CRLF, stdout);
  fputs(" U:PW  Basic Authentication string in user:password format" CRLF, stdout);
  fputs(CRLF, stdout); 
  fputs(" -?  display the command line usage info being shown now" CRLF, stdout);
  fputs(" -d  debug level (may be used more than once)" CRLF, stdout);
#ifdef OPENSSL_FIPS
  fputs(" -f  enable FIPS (Federal Information Protection Standard) encryption mode" CRLF, stdout);
#endif
  fputs(" -o  output the request and response text to a file" CRLF, stdout);
  fputs(" -s  enable transport stream security with HTTPS" CRLF, stdout);
  fputs(" -t  SSL/TLS version number for initial handshake" CRLF, stdout);
  fputs(" -v  verbosity level (may be used more than once)" CRLF, stdout);
  fputs(" -h  create an HTTP request header, duplicates permitted" CRLF, stdout);
  fputs(" -n  Set the TLS SNI (Server Name Indication) extension in ClientHello" CRLF, stdout);
  fputs(" -y  verify server-side certificate chain" CRLF, stdout);
  fputs(" -z  fuzz request line and/or request headers" CRLF, stdout);
  fputs(" -a  Basic authentication string in user:password format" CRLF, stdout);
  fputs(" -D  duplicate HTTP request headers (name,number OR number,name)" CRLF, stdout);
  fputs(" -V  show detailed version information and exit" CRLF, stdout);
  fputs(CRLF, stdout);
  printf("ex. %s www.google.com:80 GET /apps HTTP/1.1 localhost:80" CRLF, arg0);
  printf("ex. %s office365.com:443 HEAD / HTTP/1.0 -s" CRLF, arg0);
  fputs(CRLF, stdout);

  exit(EX_USAGE);
}
