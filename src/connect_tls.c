#include"fjorge.h"

BIO *connect_tls(const char *ahost, const unsigned short aport) { 
  assert(ahost);

  SSL_CTX *actx = NULL;
  BIO *aweb = NULL;
  SSL *assl = NULL;
  register int ares = 0;
  char abuf[BUFSIZ] = { 0x0 };

  sprintf(abuf, "%u", aport);

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  SSL_library_init();
#else
  OPENSSL_init_ssl(0, NULL);
#endif

  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();

  const SSL_METHOD *method = NULL;

  switch(vcmd->protocol) {
    case 1:
#ifndef DEPRECATEDIN_1_1_0
      method = TLSv1_client_method();
#else
      method = TLS_client_method();
#endif

      break;
    case 2:
#ifdef OPENSSL_NO_SSL2_METHOD
      fjputs_error("This installation of OpenSSL does not support SSLv2! (OPENSSL_NO_SSL2_METHOD)");
#else
#endif

      break;
    case 3:
#ifdef OPENSSL_NO_SSL3
      fjputs_error("This installation of OpenSSL does not support SSLv3! (OPENSSL_NO_SSL3)");
#else
      method = SSLv3_client_method();
#endif

      break;
    case 11:
#ifndef DEPRECATEDIN_1_1_0
      method = TLSv1_1_client_method();
#else
      method = TLS_client_method();
#endif

      break;
    case 12:
#ifndef DEPRECATEDIN_1_1_0
      method = TLSv1_2_client_method();
#else
      method = TLS_client_method();
#endif

      break;
    case 13:
      /* method = TLSv1_3_client_method(); */

      fjputs_error("TLSv1.3 is not implemented yet!");

      exit(EX_SOFTWARE);

      break;
    case 23:
      method = SSLv23_client_method();

      break;
    default:
      method = TLS_client_method();

      break;
  }

  if(!method)
    error_tls(assl, 0, "TLS_client_method");

  actx = SSL_CTX_new(method);

  if(!actx)
    error_tls(assl, 0, "SSL_CTX_new");

  if(vcmd->verify)
    SSL_CTX_set_info_callback(actx, info_callback);

  SSL_CTX_set_default_verify_paths(actx); 

  if(vcmd->verify)
    SSL_CTX_set_verify(actx, SSL_VERIFY_PEER, verify_callback); 
    /* SSL_CTX_set_verify(actx, SSL_VERIFY_PEER, NULL);  */
  else
    SSL_CTX_set_verify(actx, SSL_VERIFY_NONE, NULL); 

  /* const long flags = 0;  */
  const long flags = SSL_OP_ALL | SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_SINGLE_DH_USE | SSL_OP_NO_COMPRESSION; 

  SSL_CTX_set_options(actx, flags);

  aweb = BIO_new_ssl_connect(actx);

  if(!aweb)
    error_tls(assl, 0, "BIO_new_ssl_connect");

  ares = BIO_set_conn_hostname(aweb, ahost);

  if(ares <= 0)
    error_tls(assl, ares, "BIO_set_conn_hostname");

  // ares = BIO_set_conn_int_port(aweb, &aport); 
  ares = BIO_set_conn_port(aweb, abuf);

  if(ares <= 0)
    error_tls(assl, ares, "BIO_set_conn_port"); 

  BIO_get_ssl(aweb, &assl);

  if(!assl)
    error_tls(assl, 0, "BIO_get_ssl");

  const char *version = SSL_get_version(assl);

  fjprintf_verbose("Protocol: %s", version);

#if 0
  const size_t client_random_length = SSL_get_client_random(assl, NULL, 0);

  unsigned char *client_random = calloc(1, client_random_length);

  if(!client_random)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  const size_t server_random_length = SSL_get_server_random(assl, NULL, 0);

  unsigned char *server_random = calloc(1, server_random_length);

  if(!server_random)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  const SSL_SESSION *ssl_session = SSL_get_session(assl);

  if(!ssl_session)
    error_at_line(1, errno, __FILE__, __LINE__, "SSL_get_session: %s", strerror(errno));

  const size_t master_key_length = SSL_SESSION_get_master_key(ssl_session, NULL, 0);

  unsigned char *master_key = calloc(1, master_key_length);

  if(!master_key)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));
#endif

  // X509_VERIFY_PARAM *param = SSL_get0_param(assl);

  // X509_VERIFY_PARAM_set_flags(param, X509_V_FLAG_CRL_CHECK_ALL | X509_V_FLAG_X509_STRICT);

  /* X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
  X509_VERIFY_PARAM_set1_host(param, "www.google.com", 0); */
 
  if(vcmd->cipher) {
    ares = SSL_set_cipher_list(assl, vcmd->cipher);

    if(ares <= 0) {
      fjputs_error("Incorrect ciphers! Read manual page ciphers (1ssl) ..");

      exit(EX_USAGE);
    }
  } else {
    ares = SSL_set_cipher_list(assl, PREFER_CIPHERS);

    if(ares <= 0)
      error_tls(assl, ares, "SSL_set_cipher_list");
  }

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
  if(vcmd->servername)
    ares = SSL_set_tlsext_host_name(assl, vcmd->servername);
  else 
    ares = SSL_set_tlsext_host_name(assl, ahost); 
#endif

  if(ares <= 0)
    error_tls(assl, ares, "SSL_set_tlsext_host_name");

  BIO *out = BIO_new_fp(stdout, BIO_NOCLOSE);

  if(!out)
    error_tls(assl, 0, "BIO_new_fp");

  fjprintf_debug("SSL state: %s", SSL_state_string_long(assl));

  ares = BIO_do_connect(aweb);

  if(ares <= 0)
    error_tls(assl, ares, "BIO_do_connect");

  fjprintf_debug("SSL state: %s", SSL_state_string_long(assl));

  ares = BIO_do_handshake(aweb);

  if(ares <= 0)
    error_tls(assl, ares, "BIO_do_handshake");

  const X509 *peer = SSL_get_peer_certificate((const SSL *)assl);

  if(!peer)
    error_tls(assl, 0, "SSL_get_peer_certificate");

  X509_NAME *xnam = X509_get_subject_name(peer);

  if(vcmd->debug) {
    const int ecnt = X509_NAME_entry_count(xnam);

    fjprintf_debug("X509 name entry count: %d", ecnt);
  }

  int lastpos = -1;
  X509_NAME_ENTRY *e;

  for (;;) {
    lastpos = X509_NAME_get_index_by_NID(xnam, NID_commonName, lastpos);

    if (lastpos == -1)
      break;

    e = X509_NAME_get_entry(xnam, lastpos);
    ASN1_STRING *a = X509_NAME_ENTRY_get_data(e);
    const unsigned char *c = ASN1_STRING_get0_data(a);

    fjprintf_verbose("Common Name: %s", (char*)c);
  } 

/*
  const int apos = X509_NAME_get_index_by_NID(xnam, NID_commonName, -1);

  fjprintf_debug("X509 name index position: %d %m", apos);

  if(apos < 0)
    error_tls(assl, apos, "X509_NAME_get_index_by_NID");

  const X509_NAME_ENTRY *xent = X509_NAME_get_entry(xnam);
  const ASN1_STRING *asn1 = X509_NAME_ENTRY_get_data(xent);
  unsigned char *cstr = ASN1_STRING_data((ASN1_STRING *)asn1);

  fjprintf_verbose("Common Name: %s", cstr);
*/

  if(vcmd->debug) {
    X509_NAME_oneline(X509_get_subject_name(peer), abuf, sizeof abuf);
    fjprintf_debug("Subject: %s", abuf);
    X509_NAME_oneline(X509_get_issuer_name(peer), abuf, sizeof abuf);
    fjprintf_debug("Issuer: %s", abuf);
  }

  /* STACK_OF(X509) *x509 = SSL_get_peer_cert_chain(ssl);

  int i = 0;

  x509 = SSL_get_peer_cert_chain(assl);

  for(i = 0;i < sk_X509_num(x509);++i) {
    X509_NAME_oneline(X509_get_subject_name(sk_X509_value(x509, i)), abuf, sizeof abuf);
    printf("%2d s:%s\n", i, abuf);
    X509_NAME_oneline(X509_get_issuer_name(sk_X509_value(x509, i)), abuf, sizeof abuf);
    printf("   i:%s\n", abuf);
  } */

  /* const char *common_name = X509_VERIFY_PARAM_get0_peername(assl);

  if(!common_name)
    fjprintf_verbose("Common Name: %s", common_name);
  else
    fjputs_verbose("Common Name: (null)"); */

  return aweb;
}

#if 0
int main(void) {

return 0;
}
#endif
