#include"fjorge.h"

BIO *connect_tls(const char *ahost, const unsigned short aport) { 
  SSL_CTX *ctx = NULL;
  BIO *web = NULL;
  SSL *ssl = NULL;
  register int res = 0;
  char pts[BUFSIZ] = { 0x0 };

  sprintf(pts, "%u", aport);

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  SSL_library_init();
#else
  OPENSSL_init_ssl(0, NULL);
#endif

  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
  OPENSSL_config(NULL);

  /* const SSL_METHOD *method = SSLv23_client_method(); */
  /* const SSL_METHOD *method = TLSv1_2_client_method(); */
  const SSL_METHOD *method = TLS_client_method();

  if(!method)
    error_tls(ssl, 0, "SSLv23_method");

  ctx = SSL_CTX_new(method);

  if(!ctx)
    error_tls(ssl, 0, "SSL_CTX_new");

  if(vcmd->debug)
    SSL_CTX_set_info_callback(ctx, info_callback);

  SSL_CTX_set_default_verify_paths(ctx); 

  if(vcmd->verify)
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    /* SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);  */
  else
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL); 

  /* const long flags = 0;  */
  const long flags = SSL_OP_ALL | SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_SINGLE_DH_USE | SSL_OP_NO_COMPRESSION; 

  SSL_CTX_set_options(ctx, flags);

  web = BIO_new_ssl_connect(ctx);

  if(!web)
    error_tls(ssl, 0, "BIO_new_ssl_connect");

  res = BIO_set_conn_hostname(web, ahost);

  if(res <= 0)
    error_tls(ssl, res, "BIO_set_conn_hostname");

  /** TODO: try this one instead of BIO_set_conn_port **/
  /* res = BIO_set_conn_int_port(web, &aport); */
  res = BIO_set_conn_port(web, pts); 

  if(res <= 0)
    error_tls(ssl, res, "BIO_set_conn_port"); 

  BIO_get_ssl(web, &ssl);

  if(!ssl)
    error_tls(ssl, 0, "BIO_get_ssl");

  if(vcmd->cipher) 
    res = SSL_set_cipher_list(ssl, vcmd->cipher);
  else
    res = SSL_set_cipher_list(ssl, PREFER_CIPHERS);

  if(res <= 0)
    error_tls(ssl, res, "SSL_set_cipher_list");

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
  if(vcmd->servername)
    res = SSL_set_tlsext_host_name(ssl, vcmd->servername);
  else 
    res = SSL_set_tlsext_host_name(ssl, ahost); 
#endif

  if(res <= 0)
    error_tls(ssl, res, "SSL_set_tlsext_host_name");

  BIO *out = BIO_new_fp(stdout, BIO_NOCLOSE);

  if(!out)
    error_tls(ssl, 0, "BIO_new_fp");

  res = BIO_do_connect(web);

  if(res <= 0)
    error_tls(ssl, res, "BIO_do_connect");

  if(vcmd->verbose)
    fjprintf_verbose("SSL state: %s", SSL_state_string_long(ssl));

  res = BIO_do_handshake(web);

  if(res <= 0)
    error_tls(ssl, res, "BIO_do_handshake");

  return web;
}

#if 0
int main(void) {

return 0;
}
#endif
