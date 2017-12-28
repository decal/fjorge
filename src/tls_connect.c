#include"fjorge.h"

BIO *tls_connect(const char *ahost, const unsigned short aport) { 
  SSL_CTX *ctx = NULL;
  BIO *web = NULL;
  SSL *ssl = NULL;
  register int res = 0;
  char pts[BUFSIZ] = { 0x0 };

  sprintf(pts, "%u", aport);

  SSL_load_error_strings();
  SSL_library_init();

  const SSL_METHOD *method = SSLv23_client_method();

  if(!method)
    tls_error("SSLv23_method");

  ctx = SSL_CTX_new(method);

  if(!ctx)
    tls_error("SSL_CTX_new");

  SSL_CTX_set_info_callback(ctx, info_callback);
  SSL_CTX_set_default_verify_paths(ctx); 
  SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);  
  /* SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback); */

  /* const long flags = 0; */
  const long flags = SSL_OP_ALL | SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_SINGLE_DH_USE | SSL_OP_NO_COMPRESSION;

  SSL_CTX_set_options(ctx, flags);

  web = BIO_new_ssl_connect(ctx);

  if(!web)
    tls_error("BIO_new_ssl_connect");

  /* BIO_get_ssl(web, &ssl); */

  res = BIO_set_conn_hostname(web, ahost);

  if(res <= 0)
    tls_error("BIO_set_conn_hostname");

  /* res = BIO_set_conn_int_port(web, &aport); */
  res = BIO_set_conn_port(web, pts); 

  if(res <= 0)
    ssl_error(ssl, res, "BIO_set_conn_port"); 

  BIO_get_ssl(web, &ssl);

  if(!ssl)
    tls_error("BIO_get_ssl");

  res = SSL_set_cipher_list(ssl, PREFER_CIPHERS);

  if(res <= 0)
    ssl_error(ssl, res, "SSL_set_cipher_list");

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
  if(vcmd->servername)
    res = SSL_set_tlsext_host_name(ssl, vcmd->servername);
  else 
    res = SSL_set_tlsext_host_name(ssl, ahost); 
#endif

  if(res <= 0)
    ssl_error(ssl, res, "SSL_set_tlsext_host_name");

  BIO *out = BIO_new_fp(stdout, BIO_NOCLOSE);

  if(!out)
    tls_error("BIO_new_fp");

  res = BIO_do_connect(web);

  if(res <= 0)
    ssl_error(ssl, res, "BIO_do_connect");

  if(vcmd->verbose)
    fprintf(stderr, "[*] SSL state: %s\n", SSL_state_string_long(ssl));

  res = BIO_do_handshake(web);

  if(res <= 0)
    ssl_error(ssl, res, "BIO_do_handshake");

  return web;
}

#if 0
int main(void) {
return 0;
}
#endif
