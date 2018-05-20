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
#define SHA1SIZ 20
#define SERIAL_NUM_LEN BUFSIZ

#define BADGE_CALLBACK CYANB "*-*" RESET " "
#define BADGE_WRITECALLBACK CYANB "<<<" RESET " "
#define BADGE_READCALLBACK CYANB ">>>" RESET " "
#define BADGE_DEBUG WHITEB "*~*" RESET " "
#define BADGE_ERROR REDB "*!*" RESET " " 
#define BADGE_PORT CYANB "*:*" RESET " "
#define BADGE_RECV GREENB "*>*" RESET " "
#define BADGE_SEND GREENB "*<*" RESET " "
#define BADGE_TLSERROR REDB "*!*" RESET " "
#define BADGE_CALLBACKERROR REDB "*!*" RESET " "
#define BADGE_TRACE YELLOWB "*#*" RESET " " 
#define BADGE_VERBOSE BLUEB "*%*" RESET " "

/* #define PREFER_CIPHERS "ALL:!MEDIUM:!LOW:!MD5:!SHA1:!RC4:!EXPORT" */
/** @def PREFER_CIPHERS */
#define PREFER_CIPHERS "ALL"

#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<sysexits.h>
#include<signal.h>
#include<assert.h>
#include<arpa/inet.h>
#include<getopt.h>
#include<spawn.h>
#include<openssl/asn1t.h>
#include<openssl/bio.h>
#include<openssl/ocsp.h>

#if (SSLEAY_VERSION_NUMBER >= 0x0907000L)
#include<openssl/conf.h>
#endif

#define OPENSSL_THREAD_DEFINES
#include<openssl/opensslconf.h>
#if defined(OPENSSL_THREADS)
  /* thread support enabled */
#else
  /* no thread support */
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

#include"colors.h"

/** @struct Singly linked list of TCP port numbers supplied on the command line via range list syntax */
typedef struct port_numbers {
  unsigned short portlo;
  unsigned short porthi;
  struct port_numbers *nextpn;
} PORT_RANGELIST, *PPORT_RANGELIST, **PPPORT_RANGELIST;

/** @struct HTTP protocol version number from reply status line */
typedef struct protocol_version {
  char *proto;
  char *major;
  char *delim;
  char *minor;
} HTTP_VERSION, *PHTTP_VERSION, **PPHTTP_VERSION;

/** @struct Singly linked list of HTTP request headers passed on the command line */
typedef struct header_list {
  char *header;
  struct header_list *next;
} HEADER_LIST, *PHEADER_LIST, **PPHEADER_LIST;

/** @struct Singly linked list of HTTP response header names/values read from network */
typedef struct header_line {
  char *name;
  char *value;
  struct header_line *next;
} HEADER_LINE, *PHEADER_LINE, **PPHEADER_LINE;

/** @struct HTTP request(s) info passed on the command line */
typedef struct http_request {
  char *verb;
  char *path;
  char *vers;
  char *host;
  PORT_RANGELIST *prts;
  struct header_list *hdrs;
  char *body;
} HTTP_REQUEST, *PHTTP_REQUEST, **PPHTTP_REQUEST;

/** @struct Command line options after parsing */
typedef struct command_line {
  char *namein;
  FILE *input;
  char *nameout;
  FILE *output;
  char *namedir;
  DIR *dirfiles; /* requests stored in files */
  unsigned int brief; /* make displayed output brief? */
  unsigned int callback; /* information callback? (not verification) */
  unsigned int fips; /* enable Federal Information Protection Standard? */
  unsigned int verbose; /* increase output verbosity? */
  unsigned int debug; /* show debugging information? */
  unsigned int secure; /* force Transport Layer Security? */
  unsigned int multiply; /* multiple headers with same name, diff value */
  unsigned int casing; /* 1 = upper, 0 = rand, -1 = lower */
  unsigned int inject; /* inject new headers? */
  unsigned int shuffle; /* randomly shuffle request header order */
  unsigned int verify; /* verify server-side certificate? */
  unsigned int protocol;
  HTTP_VERSION version;
  HTTP_REQUEST request;
  char *attack; /* Specify attack such as XSS, SQL injection, etc. -A xss,header */
  char *basic; /* HTTP Basic authentication */
  char *cipher; /* preferred cipher set */
  char *duplicate; /* duplicate request headers */
  char *fuzz; /* fuzz request headers */
  char *hostnam; /* host name */
  char *intranet; /* private address range to scan Host: header for */
  char *reorder; /* reorder request headers */
  unsigned int portnum; /* port number */
  char *portstr; /* port number as string */
  char *portscan; /* scan ports via Host: header */
  char *servername; /* SNI (Server Name Indication) */
  char *writebuf; /* write output to 'memory buffer' or @filename */
  char *encode; /* URL encode chars */
  char *noencode; /* don't URL encode chars */
} COMMAND_LINE, *PCOMMAND_LINE, **PPCOMMAND_LINE;

/** @struct HTTP response object */
typedef struct http_response {
  unsigned short code;
  char *mesg;
  char *vers;
  char **hdrs;
  char *body;
} HTTP_RESPONSE, *PHTTP_RESPONSE, **PPHTTP_RESPONSE;

/** @struct output formatting */
typedef struct output_value {
  uint16_t http_code;    /* HTTP reply status code */
  uint16_t conn_code; /* HTTP reply status code from last proxy's CONNECT response */
  const char *http_reason; /* HTTP reply message */
  const char *conn_reason; /* HTTP reply message from last proxy's CONNECT response */
  in_addr_t local_ip; /* IP address for local end of most recent socket connection */
  in_port_t local_port; /* Port number for local end of most recent socket connection */
  uint8_t num_connects; /* Number of connections */
  uint8_t num_redirects; /* Number of redirects */
  const char *redirect_url; /* Most recent redirect URL from Location response header */
  in_addr_t remote_ip; /* IP Address for remote end of most recent socket connection */
  in_port_t remote_port; /* Port number for remote end of most recent socket connection */
  size_t size_download;
  size_t size_header;
  size_t size_request;
  size_t size_upload;
  long double speed_download;
  long double speed_upload;
  unsigned int ssl_verify_result;
  suseconds_t time_appconnect;
  suseconds_t time_connect;
  suseconds_t time_namelookup;
  suseconds_t time_pretransfer;
  suseconds_t time_redirect;
  suseconds_t time_starttransfer;
  suseconds_t time_total;
  const char *url_effective;
} OUTPUT_VALUE, *POUTPUT_VALUE, **PPOUTPUT_VALUE;

typedef struct tls_connect {
  const char *common_name;
  const char *country_name;
  const char *locality_name;
  const char *stateorprovince_name;
  const char *organization_name;
  const char *organizationalunit_name;
  const char *cipher_name;
  int cipher_bits;
  const char *cipher_description;
  const char *cipher_version;
  const char *cipher_list;
  int publickey_bits;
  const char *protocol_version;
  const char *certificate_fingerprint;
  int certificate_version;
  const char *certificate_serialnumber;
  const char *certificate_signaturealgorithm;
} TLS_CONNECT, *PTLS_CONNECT, **PPTLS_CONNECT;

typedef struct tcp_connect {
} TCP_CONNECT, *PTCP_CONNECT, **PPTCP_CONNECT;

/** @struct HTTP cookie info */
typedef struct cookie_object {
  const char *name;
  const char *value;
  const char *max_age; // time_t max_age;
  const char *domain;
  const char *path;
  const char *expires; // struct tm *expires;
  unsigned int secure;
  unsigned int httponly;
  const char *samesite; /* "lax" or "strict" */
} COOKIE_OBJECT, *PCOOKIE_OBJECT, **PPCOOKIE_OBJECT;

typedef int (*cookie_cb)(const char *, const char *, const char *, const char *, const struct tm *);

/** @struct callback function pointers */
typedef struct callback_funcptrs { 
  cookie_cb set_cookie;
} CALLBACK_FUNCPTRS, *PCALLBACK_FUNCPTRS, **PPCALLBACK_FUNCPTRS;

extern CALLBACK_FUNCPTRS *cbak;
extern COMMAND_LINE *vcmd;
extern OUTPUT_VALUE *outv;
extern BIO *bioout;
extern BIO *bioerr;
extern BIO *bioin;
extern char *ahost;
extern char *bhost;
extern char *chost;

void setcb_cookie(cookie_cb);
noreturn void signal_handler(const int);
HEADER_LIST *add_header(const char *);
void callback_message(int, int, int, const void *, size_t, SSL *, void *);
char *decode_base64(const char *);
void dup_headers(const char *);
char *encode_base64(const char *);
void encode_hex(unsigned char *, void *, const size_t);
void auth_basic(const char *);
signed char **make_hostnames(char **, const char *restrict *const, size_t);
BIO *create_sockbio(const int, const HTTP_REQUEST *);
char *create_serial(X509 *);
int connect_tcp(const char *, const unsigned short);
int close_tcp(const int);
BIO *connect_tls(const char *, const unsigned short);
int error_callback(const unsigned long, const char *);
void msg_callback(int, int, int, const void *, size_t, SSL *, void *);
void callback_info(const SSL *, int, int);
size_t count_portlist(PORT_RANGELIST *);
size_t count_portstr(const char *);
PORT_RANGELIST *parse_portstr(const char *);
int callback_ocsp(SSL *, void *);
int callback_verify(int, X509_STORE_CTX *);
BIO *error_tls(const SSL *, const int, const char *const);
int error_tcp(const char *);
size_t recv_response(BIO *);
int send_request(BIO *, const HTTP_REQUEST *);
HTTP_VERSION *unpack_protover(const char *);
char *pack_protover(const HTTP_VERSION *);
void parse_cmdline(const int, const char **);
noreturn void describe_usage(const char *const restrict); 
const size_t array_length(char **);
unsigned short *array_portlist(PORT_RANGELIST *);
unsigned short *array_portstr(const char *);
signed char **print_hostnames(const char *restrict *const, const char *restrict *const, size_t);
void print_options(FILE *);
void print_trace(void);
HEADER_LINE *extract_header(char *restrict);
long fjcb_bio_debug(BIO *, int, const char *, int, long, long);
int fjprintf_callback(const char *, ...);
int fjprintf_debug(const char *, ...);
int fjprintf_error(const char *, ...);
int fjprintf_verbose(const char *, ...);
int fjputs_callback(const char *const);
int fjputs_debug(const char *const);
int fjputs_error(const char *const);
int fjputs_verbose(const char *const);
char *rewrite_arg2head(char *);
unsigned char *output_x509nm(const char *label, const X509_NAME *const, const int);
#endif
