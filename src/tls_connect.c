#include"fjorge.h"

static void tls_error(const char *afunc) {
  perror(afunc);

  exit(EXIT_FAILURE);
}

BIO *tls_connect(const char *ahost, const unsigned short aport) { 
  SSL_CTX *ctx = NULL;
  BIO *web = NULL;
  SSL *ssl = NULL;
  register int res = 0;

  SSL_library_init();

  const SSL_METHOD *method = SSLv23_method();

  if(!method)
    tls_error("SSLv23_method");

  ctx = SSL_CTX_new(method);

  if(!ctx)
    tls_error("SSL_CTX_new");

  SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

  /* const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION; */
  const long flags = 0;

  SSL_CTX_set_options(ctx, flags);

  web = BIO_new_ssl_connect(ctx);

  if(!web)
    tls_error("BIO_new_ssl_connect");

  BIO_get_ssl(web, ssl);

  res = BIO_set_conn_hostname(web, ahost);

  if(res <= 0)
    tls_error("BIO_set_conn_hostname");

  res = BIO_set_conn_int_port(web, &aport);

  if(res <= 0)
    tls_error("BIO_set_conn_int_port");

  BIO_get_ssl(web, &ssl);

  if(!ssl)
    tls_error("BIO_get_ssl");

  res = SSL_set_cipher_list(ssl, PREFER_CIPHERS);

  if(res <= 0)
    tls_error("SSL_set_cipher_list");

  res = SSL_set_tlsext_host_name(ssl, ahost);

  if(res <= 0)
    tls_error("SSL_set_tlsext_host_name");

  /* out = BIO_new_fp(stdout, BIO_NOCLOSE);

  if(!out)
    tls_error("BIO_new_fp"); */

  res = BIO_do_connect(web);

  if(res <= 0)
    tls_error("BIO_do_connect");

  res = SSL_do_handshake(ssl);

  if(res <= 0)
    tls_error("SSL_do_handshake");

  return web;
}

#if 0
int main(void) {
return 0;
}
#endif
