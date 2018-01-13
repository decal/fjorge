#ifndef _FJORGE_H
#define _POSIX_C_SOURCE 1
#define _BSD_SOURCE _POSIX_C_SOURCE
#define _FJORGE_H _BSD_SOURCE
#define NDEBUG _FJORGE_H
/* #define _FORITY_SOURCE NDEBUG  */

#define noreturn _Noreturn
#define CRLF "\r\n"

#define HTT1 "HTTP/1.0"
#define HT11 "HTTP/1.1"
#define HT09 "HTTP/0.9"
#define HT20 "HTTP/2.0"
#define HTT2 HT20

#define DEFAULT_HTTPS_PORT 443u
#define DEFAULT_HTTP_PORT 80u

#define BADGE_CALLBACK "*-* "
#define BADGE_DEBUG "*~* "
#define BADGE_ERROR "*!* "
#define BADGE_RECV "*>* "
#define BADGE_SEND "*<* "
#define BADGE_TLSERROR "*=* "
#define BADGE_TRACE "*#* "
#define BADGE_VERBOSE "*%* "

/* #define PREFER_CIPHERS "ALL:!MEDIUM:!LOW:!MD5:!SHA1:!RC4:!EXPORT" */
#define PREFER_CIPHERS "ALL"

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<sysexits.h>
#include<signal.h>
#include<assert.h>
#include<arpa/inet.h>
#include<getopt.h>
#include<openssl/asn1t.h>
#include<openssl/bio.h>

#if (SSLEAY_VERSION_NUMBER >= 0x0907000L)
#include<openssl/conf.h>
#endif

#include<openssl/err.h> 
#include<openssl/ssl.h>
#include<openssl/x509.h>
#include<openssl/x509v3.h> 
#include<openssl/opensslv.h>
#include<openssl/crypto.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/queue.h>
#include<dirent.h>
#include<unistd.h>
#include<netdb.h>
#include<linux/in.h>
#include<execinfo.h>
#include<ctype.h>
#include<errno.h>
#include<error.h>
#include<zlib.h>

typedef enum {
  MatchFound,
  MatchNotFound,
  NoSANPresent,
  MalformedCertificate,
  Error
} HostnameValidationResult;

typedef struct {
  int verbose_mode;
  int verify_depth;
  int always_continue;
} mydata_t;

typedef struct protocol_version {
  char *proto;
  char *major;
  char *delim;
  char *minor;
} PROTOCOL_VERSION, *PPROTOCOL_VERSION, **PPPROTOCOL_VERSION;

typedef struct linked_list {
  char *header;
  struct linked_list *next;
} LINKED_LIST, *PLINKED_LIST, **PPLINKED_LIST;

typedef struct http_request {
  char *verb;
  char *path;
  char *vers;
  char *host;
  struct linked_list *hdrs;
  char *body;
} HTTP_REQUEST, *PHTTP_REQUEST, **PPHTTP_REQUEST;

typedef struct command_line {
  char *namein;
  FILE *input;
  char *nameout;
  FILE *output;
  char *namedir;
  DIR *dirfiles; /* requests stored in files */
  unsigned int brief; /* make displayed output brief? */
  unsigned int callback; /* information callback? (not verification) */
  unsigned int duplicate; /* duplicate request headers? */
  unsigned int fips; /* enable Federal Information Protection Standard? */
  unsigned int verbose; /* increase output verbosity? */
  unsigned int debug; /* show debugging information? */
  unsigned int secure; /* force Transport Layer Security? */
  unsigned int fuzz; /* fuzz request headers? */
  unsigned int shuffle; /* reorder request headers? */
  unsigned int multiply; /* multiple headers with same name, diff value */
  unsigned int casing; /* 1 = upper, 0 = rand, -1 = lower */
  unsigned int inject; /* inject new headers? */
  unsigned int verify; /* verify server-side certificate? */
  PROTOCOL_VERSION protocol;
  HTTP_REQUEST request;
  char *attack; /* Specify attack such as XSS, SQL injection, etc. -A xss,header */
  char *basic; /* HTTP Basic authentication */
  char *cipher; /* preferred cipher set */
  char *hostnam; /* host name */
  unsigned int portnum; /* port number */
  char *scan_ports; /* scan ports via Host: header */
  char *servername; /* SNI (Server Name Indication) */
} COMMAND_LINE, *PCOMMAND_LINE, **PPCOMMAND_LINE;

typedef struct http_response {
  unsigned short code;
  char *mesg;
  char *vers;
  char **hdrs;
  char *body;
} HTTP_RESPONSE, *PHTTP_RESPONSE, **PPHTTP_RESPONSE;

extern COMMAND_LINE *vcmd;

noreturn void signal_handler(const int);
void add_header(char *restrict);
char *decode_base64(const char *);
char *encode_base64(const char *);
void auth_basic(const char *);
signed char **make_hostnames(char **, const char *restrict *const, size_t);
FILE *send_request(const int, const HTTP_REQUEST *);
size_t recv_response(FILE *);
int connect_tcp(const char *, const unsigned short);
int close_tcp(const int);
BIO *connect_tls(const char *, const unsigned short);
void error_callback(const unsigned long, const char *const);
void info_callback(const SSL *, int, int);
HostnameValidationResult match_cn(const char *, const X509 *);
HostnameValidationResult match_san(const char *, const X509 *);
int verify_callback(int, X509_STORE_CTX *);
void error_tls(const SSL *, const int, const char *const);
size_t recv_tls(BIO *);
int send_tls(BIO *, const HTTP_REQUEST *);
PROTOCOL_VERSION *unpack_protover(const char *);
char *pack_protover(const PROTOCOL_VERSION *);
void parse_cmdline(const int, const char **);
noreturn void usage_desc(const char *const restrict); 
const size_t array_length(char **);
signed char **print_hostnames(const char *restrict *const, const char *restrict *const, size_t);
void print_options(FILE *);
void print_trace(void);
int fjprintf_callback(const char *, ...);
int fjprintf_debug(const char *, ...);
int fjprintf_error(const char *, ...);
int fjprintf_verbose(const char *, ...);
int fjputs_callback(const char *);
int fjputs_debug(const char *);
int fjputs_error(const char *);
int fjputs_verbose(const char *);
void cbprint_cnname(const char *label, const X509_NAME *const);
void cbprint_sanname(const char *label, const X509 *const);
#endif
