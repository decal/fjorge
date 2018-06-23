#include"fjorge.h"

noreturn void describe_usage(const char *const restrict arg0) {
  printf(NEWL YELLOWB "usage" RESET WHITEF ":" GREENF " %s" RESET " " BLUEB BLACKF "HOST[:PORT]" RESET " " BLUEB WHITEF "VERB" RESET " " BLUEB WHITEF "PATH" RESET " " BLUEB BLACKF "VERS" RESET " " BLUEB WHITEF "[HHST[:HPRT]]" RESET " " BLUEB WHITEF "[MXFW]" RESET " " BLUEB BLACKF "[-d]" RESET " " BLUEB BLACKF "[-s]" RESET " " BLUEB BLACKF "[-v]" RESET " " BLUEB BLACKF "[-y]" RESET " " BLUEB BLACKF "[-z]" RESET " " BLUEB BLACKF "[-n D.OM]" RESET " " BLUEB BLACKF "[-h 'NAME: VALU']" RESET " " BLUEB BLACKF "[-o OFIL]" RESET " " BLUEB BLACKF "[-B U:PW]" RESET " " BLUEB BLACKF "[-D [[NAME,DUPS]|[DUPS,NAME]]" RESET NEWL, arg0);
  fputs(NEWL, stdout);
  fputs(" " BLUEB BLACKF "HOST" RESET "  " YELLOWF "numeric IP address or DNS hostname of target web server" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "PORT" RESET "  " YELLOWF "TCP port number to connect to on destination server" RESET NEWL, stdout);
  fputs(" " BLUEB WHITEF "VERB" RESET "  " YELLOWF "HTTP protocol method: GET, HEAD, POST, OPTIONS, CONNECT" RESET NEWL, stdout);
  fputs(" " BLUEB WHITEF "PATH" RESET "  " YELLOWF "relative path string or fully qualified URL" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "VERS" RESET "  " YELLOWF "protocol version string such as: HTTP/1.0, HTTP/2.0, etc." RESET NEWL, stdout);
  fputs(" " BLUEB WHITEF "HHST" RESET "  " YELLOWF "the hostname part of a custom Host header" RESET NEWL, stdout);
  fputs(" " BLUEB WHITEF "HPRT" RESET "  " YELLOWF "the port number part of a custom Host header" RESET NEWL, stdout);
  fputs(" " BLUEB WHITEF "MXFW" RESET "  " YELLOWF "value of Max-Forwards request header" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "NAME" RESET "  " YELLOWF "HTTP request header field name" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "VALU" RESET "  " YELLOWF "string value paired up with the new header name" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "OFIL" RESET "  " YELLOWF "path name of output file to write HTTP traffic to" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "DUPS" RESET "  " YELLOWF "number of duplicate HTTP request headers to create" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "D.OM" RESET "  " YELLOWF "hostname for SNI (Server Name Indication)" RESET NEWL, stdout);
  fputs(" " BLUEB BLACKF "U:PW" RESET "  " YELLOWF "Basic Authentication string in user:password format" RESET NEWL, stdout);
  fputs(NEWL, stdout); 
  fputs(" " GREENF "-?" "  " YELLOWF "display the command line usage info being shown now" NEWL, stdout);
  fputs(" " GREENF "-d" "  " YELLOWF "debug level (may be used more than once)" NEWL, stdout);
  fputs(" " GREENF "-e" "  " YELLOWF "ensure that these characters are encoded (base64, cgi, html)" NEWL, stdout);
#ifdef OPENSSL_FIPS
  fputs(" " GREENF "-f" "  " YELLOWF "enable FIPS (Federal Information Protection Standard) encryption mode" NEWL, stdout);
#endif
  fputs(" " GREENF "-o" "  " YELLOWF "output the request and response text to a file" NEWL, stdout);
  fputs(" " GREENF "-s" "  " YELLOWF "enable transport stream security with HTTPS" NEWL, stdout);
  fputs(" " GREENF "-t" "  " YELLOWF "SSL/TLS version number for initial handshake" NEWL, stdout);
  fputs(" " GREENF "-v" "  " YELLOWF "verbosity level (may be used more than once)" NEWL, stdout);
  fputs(" " GREENF "-h" "  " YELLOWF "create an HTTP request header, duplicates permitted" NEWL, stdout);
  fputs(" " GREENF "-n" "  " YELLOWF "Set the TLS SNI (Server Name Indication) extension in " WHITEF "ClientHello" RESET NEWL, stdout);
  fputs(" " GREENF "-w" "  " YELLOWF "display custom-formatted output string template based on results" NEWL, stdout);
  fputs(" " GREENF "-y" "  " YELLOWF "verify server-side certificate chain" NEWL, stdout);
  fputs(" " GREENF "-z" "  " YELLOWF "fuzz request line and/or request headers" NEWL, stdout);
  fputs(" " GREENF "-a" "  " YELLOWF "Basic authentication string in " RESET BLUEB BLACKF "U:PW" RESET YELLOWF " format" NEWL, stdout);
  fputs(" " GREENF "-b" "  " YELLOWF "brief ouput without document body; minimizes displayed text" NEWL, stdout);
  fputs(" " GREENF "-c" "  " YELLOWF "provide cipher preferences according to " WHITEF "CIPHER STRINGS" YELLOWF " section of " WHITEF "ciphers(1)" YELLOWF " manual" NEWL, stdout);
  fputs(" " GREENF "-A" "  " YELLOWF "specify one or more attack types to unleash against the target" NEWL, stdout);
  fputs(" " GREENF "-D" "  " YELLOWF "duplicate HTTP request headers (name,number OR number,name)" NEWL, stdout);
  fputs(" " GREENF "-E" "  " YELLOWF "do not encode this set of characters (base64, cgi, html)" NEWL, stdout);
  fputs(" " GREENF "-F" "  " YELLOWF "enable random fuzzing of HTTP application layer protocol data" NEWL, stdout);
  fputs(" " GREENF "-P" "  " YELLOWF "scan Host header for private address range (10, 172, 192)" NEWL, stdout);
  fputs(" " GREENF "-V" "  " YELLOWF "show detailed version information and exit" NEWL, stdout);
  fputs(RESET NEWL, stdout);
  printf(INVON "ex." INVOFF GREENF " %s www.google.com:80 GET /apps HTTP/1.1 localhost:80" RESET NEWL, arg0);
  printf(INVON "ex." INVOFF GREENF " %s office365.com:443 HEAD / HTTP/1.0 -s" RESET NEWL, arg0);
  fputc('\n', stdout);

  exit(EX_USAGE);
}
