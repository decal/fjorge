#include"fjorge.h"

noreturn void describe_usage(const char *const restrict arg0) {
  printf(CRLF YELLOWB "usage" RESET WHITEF ":" GREENF " %s" RESET " " BLUEB "HOST[:PORT]" RESET " " BLUEB "VERB" RESET " " BLUEB "PATH" RESET " " BLUEB "VERS" RESET " " BLUEB "[HHST[:HPRT]]" RESET " " BLUEB "[-d]" RESET " " BLUEB "[-s]" RESET " " BLUEB "[-v]" RESET " " BLUEB "[-y]" RESET " " BLUEB "[-z]" RESET " " BLUEB "[-n D.OM]" RESET " " BLUEB "[-h 'NAME: VALU']" RESET " " BLUEB "[-o OFIL]" RESET " " BLUEB "[-B U:PW]" RESET " " BLUEB "[-D [[NAME,DUPS]|[DUPS,NAME]]" RESET CRLF, arg0);
  fputs(CRLF, stdout);
  fputs(" " BLUEB "HOST" RESET "  " YELLOWF "numeric IP address or DNS hostname of target web server" RESET CRLF, stdout);
  fputs(" " BLUEB "PORT" RESET "  " YELLOWF "TCP port number to connect to on destination server" RESET CRLF, stdout);
  fputs(" " BLUEB "VERB" RESET "  " YELLOWF "HTTP protocol method: GET, HEAD, POST, OPTIONS, CONNECT" RESET CRLF, stdout);
  fputs(" " BLUEB "PATH" RESET "  " YELLOWF "relative path string or fully qualified URL" RESET CRLF, stdout);
  fputs(" " BLUEB "VERS" RESET "  " YELLOWF "protocol version string such as: HTTP/1.0, HTTP/2.0, etc." RESET CRLF, stdout);
  fputs(" " BLUEB "HHST" RESET "  " YELLOWF "the hostname part of a custom Host header" RESET CRLF, stdout);
  fputs(" " BLUEB "HPRT" RESET "  " YELLOWF "the port number part of a custom Host header" RESET CRLF, stdout);
  fputs(" " BLUEB "NAME" RESET "  " YELLOWF "HTTP request header field name" RESET CRLF, stdout);
  fputs(" " BLUEB "VALU" RESET "  " YELLOWF "string value paired up with the new header name" RESET CRLF, stdout);
  fputs(" " BLUEB "OFIL" RESET "  " YELLOWF "path name of output file to write HTTP traffic to" RESET CRLF, stdout);
  fputs(" " BLUEB "DUPS" RESET "  " YELLOWF "number of duplicate HTTP request headers to create" RESET CRLF, stdout);
  fputs(" " BLUEB "D.OM" RESET "  " YELLOWF "hostname for SNI (Server Name Indication)" RESET CRLF, stdout);
  fputs(" " BLUEB "U:PW" RESET "  " YELLOWF "Basic Authentication string in user:password format" RESET CRLF, stdout);
  fputs(CRLF, stdout); 
  fputs(" " GREENF "-?" "  " YELLOWF "display the command line usage info being shown now" CRLF, stdout);
  fputs(" " GREENF "-d" "  " YELLOWF "debug level (may be used more than once)" CRLF, stdout);
  fputs(" " GREENF "-e" "  " YELLOWF "ensure that these characters are encoded (base64, cgi, html)" CRLF, stdout);
#ifdef OPENSSL_FIPS
  fputs(" " GREENF "-f" "  " YELLOWF "enable FIPS (Federal Information Protection Standard) encryption mode" CRLF, stdout);
#endif
  fputs(" " GREENF "-o" "  " YELLOWF "output the request and response text to a file" CRLF, stdout);
  fputs(" " GREENF "-s" "  " YELLOWF "enable transport stream security with HTTPS" CRLF, stdout);
  fputs(" " GREENF "-t" "  " YELLOWF "SSL/TLS version number for initial handshake" CRLF, stdout);
  fputs(" " GREENF "-v" "  " YELLOWF "verbosity level (may be used more than once)" CRLF, stdout);
  fputs(" " GREENF "-h" "  " YELLOWF "create an HTTP request header, duplicates permitted" CRLF, stdout);
  fputs(" " GREENF "-n" "  " YELLOWF "Set the TLS SNI (Server Name Indication) extension in " WHITEF "ClientHello" RESET CRLF, stdout);
  fputs(" " GREENF "-w" "  " YELLOWF "display custom-formatted output string template based on results" CRLF, stdout);
  fputs(" " GREENF "-y" "  " YELLOWF "verify server-side certificate chain" CRLF, stdout);
  fputs(" " GREENF "-z" "  " YELLOWF "fuzz request line and/or request headers" CRLF, stdout);
  fputs(" " GREENF "-a" "  " YELLOWF "Basic authentication string in " RESET BLUEB "U:PW" RESET YELLOWF " format" CRLF, stdout);
  fputs(" " GREENF "-b" "  " YELLOWF "brief ouput without document body; minimizes displayed text" CRLF, stdout);
  fputs(" " GREENF "-c" "  " YELLOWF "provide cipher preferences according to " WHITEF "CIPHER STRINGS" YELLOWF " section of " WHITEF "ciphers(1)" YELLOWF " manual" CRLF, stdout);
  fputs(" " GREENF "-A" "  " YELLOWF "specify one or more attack types to unleash against the target" CRLF, stdout);
  fputs(" " GREENF "-D" "  " YELLOWF "duplicate HTTP request headers (name,number OR number,name)" CRLF, stdout);
  fputs(" " GREENF "-E" "  " YELLOWF "do not encode this set of characters (base64, cgi, html)" CRLF, stdout);
  fputs(" " GREENF "-F" "  " YELLOWF "enable random fuzzing of HTTP application layer protocol data" CRLF, stdout);
  fputs(" " GREENF "-P" "  " YELLOWF "scan Host header for private address range (10, 172, 192)" CRLF, stdout);
  fputs(" " GREENF "-V" "  " YELLOWF "show detailed version information and exit" CRLF, stdout);
  fputs(RESET CRLF, stdout);
  printf(INVON "ex." INVOFF GREENF " %s www.google.com:80 GET /apps HTTP/1.1 localhost:80" RESET CRLF, arg0);
  printf(INVON "ex." INVOFF GREENF " %s office365.com:443 HEAD / HTTP/1.0 -s" RESET CRLF, arg0);
  fputs(CRLF, stdout);

  exit(EX_USAGE);
}
