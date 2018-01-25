#include"fjorge.h"

BIO *error_tls(const SSL *ssl, const int ret, const char *const afn) {
  assert(afn);

  if(!ssl) {
    perror(afn);

    return NULL;
  }
    
  const int res = SSL_get_error(ssl, ret);

  fputs(BADGE_TLSERROR, stderr);
  fputs(afn, stderr);
  fputs(": ", stderr);

  switch(res) {
    case SSL_ERROR_NONE:
      fputs("SSL_ERROR_NONE", stderr);
      
      break;
    case SSL_ERROR_ZERO_RETURN:
      fputs("SSL_ERROR_ZERO_RETURN", stderr);

      break;
    case SSL_ERROR_WANT_READ:
      fputs("SSL_ERROR_WANT_READ", stderr);

      break;
    case SSL_ERROR_WANT_WRITE:
      fputs("SSL_ERROR_WANT_WRITE", stderr);

      break;
    case SSL_ERROR_WANT_CONNECT:
      fputs("SSL_ERROR_WANT_CONNECT", stderr);

      break;
    case SSL_ERROR_WANT_ACCEPT:
      fputs("SSL_ERROR_WANT_ACCEPT", stderr);

      break;
    case SSL_ERROR_WANT_X509_LOOKUP:
      fputs("SSL_ERROR_WANT_X509_LOOKUP", stderr);

      break;
    case SSL_ERROR_SYSCALL:
      fputs("SSL_ERROR_SYSCALL", stderr);

      break;
    case SSL_ERROR_SSL:
      fputs("SSL_ERROR_SSL", stderr);

      break;
    default:
      break;
  }

  fputc(' ', stderr);
  fputs(SSL_state_string_long(ssl), stderr);
  fputs(CRLF, stderr);

  return NULL;
}
