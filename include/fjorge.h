#ifndef _FJORGE_H
#define _POSIX_C_SOURCE 1
#define _BSD_SOURCE _POSIX_C_SOURCE
#define _FJORGE_H _BSD_SOURCE
#define NDEBUG _FJORGE_H
/* #define _FORITY_SOURCE NDEBUG  */
#define CRLF "\r\n"
#define HTT1 "HTTP/1.0"
#define HT11 "HTTP/1.1"
#define HT09 "HTTP/0.9"
#define HT20 "HTTP/2.0"
#define HTT2 HT20
/* #define PREFER_CIPHERS "ALL:!MEDIUM:!LOW:!MD5:!SHA1:!RC4:!EXPORT" */
#define PREFER_CIPHERS "ALL"

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<sysexits.h>
#include<signal.h>
#include<assert.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<getopt.h>
#include<openssl/bio.h>
#include<openssl/ssl.h>
#include<openssl/x509.h>
#include<openssl/opensslv.h>
#include<openssl/crypto.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/queue.h>
#include<dirent.h>
#include<unistd.h>
#include<netdb.h>
#include<execinfo.h>
#include<ctype.h>
#include<errno.h>
#include<error.h>
#include<zlib.h>

typedef struct protocol_version {
  char *proto;
  char *major;
  char *delim;
  char *minor;
} PROTOCOL_VERSION, *PPROTOCOL_VERSION;

typedef struct linked_list {
  char *restrict header;
  struct linked_list *next;
} LINKED_LIST, *PLINKED_LIST;

typedef struct http_request {
  char *verb;
  char *path;
  char *vers;
  char *host;
  struct linked_list*hdrs;
  char *body;
} HTTP_REQUEST, *PHTTP_REQUEST;

typedef struct command_line {
  char *namein;
  FILE *input;
  char *nameout;
  FILE *output;
  char *namedir;
  DIR *dirfiles; /* requests stored in files */
  unsigned int brief;
  unsigned int verbose;
  unsigned int debug;
  unsigned int secure;
  unsigned int fuzz;
  unsigned int shuffle; /* reorder request headers */
  unsigned int multiply; /* multiple headers with same name, diff value */
  unsigned int casing; /* 1 = upper, 0 = rand, -1 = lower */
  unsigned int inject;
  unsigned int remove;
  PROTOCOL_VERSION protocol;
  HTTP_REQUEST request;
  char *basic;
  char *hostnam;
  unsigned int portnum;
} COMMAND_LINE, *PCOMMAND_LINE;

typedef struct http_response {
  unsigned short code;
  char *mesg;
  char *vers;
  char **hdrs;
  char *body;
} HTTP_RESPONSE, *PHTTP_RESPONSE;

extern COMMAND_LINE *vcmd;

_Noreturn void signal_handler(const int);
unsigned int add_header(char*restrict);
char *base64_encode(const char*);
unsigned int basic_auth(const char *);
FILE *send_request(const int, const HTTP_REQUEST*);
int tls_send_request(BIO*, const HTTP_REQUEST*);
size_t recv_response(FILE*);
size_t tls_recv_response(BIO*);
int tcp_connect(const char*, const unsigned short);
int tcp_close(const int);
BIO *tls_connect(const char*, const unsigned short);
PROTOCOL_VERSION *unpack_protover(const char*);
char *pack_protover(const PROTOCOL_VERSION*);
COMMAND_LINE *parse_cmdline(const int, const char**);
_Noreturn void usage_desc(const char *const restrict);
size_t array_length(char **);
void print_options(FILE*);
void print_trace(void);
#endif
