#include"fjorge.h"

BIO *connect_tls(const char *ahost, const unsigned short aport) { 
  assert(ahost);
  assert(aport);

  SSL_CTX *actx = NULL;
  BIO *aweb = NULL;
  SSL *assl = NULL;
  TLS_CONNECT *tcon = NULL;
  register int ares = 0;
  char abuf[BUFSIZ] = { 0x0 };

  sprintf(abuf, "%u", (unsigned int)aport);

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
      method = TLSv1_client_method();

      break;
  }

  if(!method)
    return error_tls(assl, 0, "TLS_client_method");

  actx = SSL_CTX_new(method);

  if(!actx)
    return error_tls(assl, 0, "SSL_CTX_new");

  SSL_CTX_set_session_cache_mode(actx, SSL_SESS_CACHE_NO_AUTO_CLEAR | SSL_SESS_CACHE_BOTH);

  if(vcmd->verbose) {
    const long cmod = SSL_CTX_get_session_cache_mode(actx);

    if((cmod & SSL_SESS_CACHE_OFF))
      fjputs_verbose("No session caching takes place");

    if((cmod & SSL_SESS_CACHE_CLIENT))
      fjputs_verbose("Client sessions are added to the cache");

    if((cmod & SSL_SESS_CACHE_SERVER))
      fjputs_verbose("Server sessions are added to the cache");

    if((cmod & SSL_SESS_CACHE_NO_AUTO_CLEAR))
      fjputs_verbose("Session cache automatic flushing is disabled");

    if((cmod & SSL_SESS_CACHE_NO_INTERNAL_LOOKUP))
      fjputs_verbose("No lookups in internal session cache");

    if((cmod & SSL_SESS_CACHE_NO_INTERNAL_STORE))
      fjputs_verbose("No storage in internal session cache");

    if((cmod & SSL_SESS_CACHE_NO_INTERNAL))
      fjputs_verbose("No lookups or storage in internal session cache");

    SSL_CTX_set_tlsext_status_cb(actx, callback_ocsp);
    SSL_CTX_set_tlsext_status_arg(actx, bioerr);
  }

  //if(vcmd->verify) {
    // SSL_CTX_set_info_callback(actx, callback_info);
    // SSL_CTX_set_msg_callback(actx, msg_callback);
  //}

  SSL_CTX_set_default_verify_paths(actx); 

  if(vcmd->verify)
    SSL_CTX_set_verify(actx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, callback_verify); 
    /* SSL_CTX_set_verify(actx, SSL_VERIFY_PEER, NULL);  */
  else
    SSL_CTX_set_verify(actx, SSL_VERIFY_NONE, NULL); 

  const long flags = SSL_OP_ALL | SSL_OP_CIPHER_SERVER_PREFERENCE | SSL_OP_SINGLE_DH_USE | SSL_OP_NO_COMPRESSION; 

  SSL_CTX_set_options(actx, flags);

  aweb = BIO_new_ssl_connect(actx);

  if(!aweb)
    return error_tls(assl, 0, "BIO_new_ssl_connect");

  ares = BIO_set_conn_hostname(aweb, ahost);

  if(ares <= 0)
    return error_tls(assl, ares, "BIO_set_conn_hostname");

  ares = BIO_set_conn_port(aweb, abuf);

  if(ares <= 0)
    return error_tls(assl, ares, "BIO_set_conn_port"); 

  BIO_get_ssl(aweb, &assl);

  if(!assl)
    return error_tls(assl, 0, "BIO_get_ssl");

  tcon = calloc(1, sizeof *tcon);

  if(!tcon)
    exit_verbose("calloc", __FILE__, __LINE__);

  //if(0) {
    /* unsigned char protos[] = {
      6, 's', 'p', 'd', 'y', '/', '1',
      8, 'h', 't', 't', 'p', '/', '0', '.', '9',
      8, 'h', 't', 't', 'p', '/', '1', '.', '0',
      8, 'h', 't', 't', 'p', '/', '1', '.', '1'
    }; */

    /* SSL_set_debug(aweb, 1);
    SSL_set_alpn_protos(assl, protos, sizeof protos);
    BIO_set_ssl_renegotiate_bytes(aweb, 512);
    SSL_enable_ct(assl, SSL_CT_VALIDATION_PERMISSIVE);
    SSL_set_tlsext_status_type(assl, TLSEXT_STATUSTYPE_ocsp);
    SSL_set_msg_callback(assl, callback_message);
    SSL_set_msg_callback_arg(assl, bioout); */
  //}

  tcon->protocol_version = SSL_get_version(assl);

  fjprintf_verbose("Protocol: %s", tcon->protocol_version);

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

  X509_VERIFY_PARAM *param = SSL_get0_param(assl);

  X509_VERIFY_PARAM_set_flags(param, X509_V_FLAG_CRL_CHECK_ALL | X509_V_FLAG_X509_STRICT);

  X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
  // X509_VERIFY_PARAM_set1_host(param, "www.google.com", 0); 
 
  if(vcmd->cipher) {
    tcon->cipher_list = vcmd->cipher;

    ares = SSL_set_cipher_list(assl, vcmd->cipher);

    if(ares <= 0) {
      fjputs_error("Incorrect ciphers! Read manual page ciphers (1ssl) ..");

      exit(EX_USAGE);
    }
  } else {
    tcon->cipher_list = PREFER_CIPHERS;

    ares = SSL_set_cipher_list(assl, PREFER_CIPHERS);

    if(ares <= 0)
      return error_tls(assl, ares, "SSL_set_cipher_list");
  }

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
  if(vcmd->servername)
    ares = SSL_set_tlsext_host_name(assl, vcmd->servername);
  else 
    ares = SSL_set_tlsext_host_name(assl, ahost); 

  if(ares <= 0)
    return error_tls(assl, ares, "SSL_set_tlsext_host_name");
#endif

  fjprintf_debug("SSL state: %s", SSL_state_string_long(assl));

  ares = BIO_do_connect(aweb);

  fjprintf_debug("SSL state: %s", SSL_state_string_long(assl));

  if(ares <= 0)
    return error_tls(assl, ares, "BIO_do_connect");

  fjprintf_debug("SSL state: %s", SSL_state_string_long(assl));

  ares = BIO_do_handshake(aweb);

  if(ares <= 0)
    return error_tls(assl, ares, "BIO_do_handshake");

  X509 *peer = SSL_get_peer_certificate((const SSL *)assl);

  if(!peer)
    return error_tls(assl, 0, "SSL_get_peer_certificate");

  // printf("result: %ld\n", SSL_get_verify_result(assl));

  X509_NAME *xnam = X509_get_subject_name(peer);

  if(!xnam)
    return error_tls(assl, 0, "X509_get_subject_name");

  if(vcmd->debug) {
    const int ecnt = X509_NAME_entry_count(xnam);

    fjprintf_debug("X509 name entry count: %d", ecnt);
  }

  if(vcmd->verbose) {
    if(peer) {
      EVP_PKEY *pktmp = X509_get_pubkey(peer);

      fjprintf_verbose("Server public key is %d bit", EVP_PKEY_bits(pktmp));
      EVP_PKEY_free(pktmp);
    }

    if(SSL_get_secure_renegotiation_support(assl))
      fjputs_verbose("Secure Renegotiation IS supported");
    else
      fjputs_verbose("Secure Renegotiation IS NOT supported");

    if(SSL_session_reused(assl))
      fjputs_verbose("TLS session WAS reused");
    else
      fjputs_verbose("TLS session WAS NOT reused");

    if(SSL_check_private_key(assl))
      fjputs_verbose("Private key IS consistent");
    else
      fjputs_verbose("Private key IS NOT consistent");

    /* if(SSL_ct_is_enabled(assl))
      fjputs_verbose("Certificate Transparency IS enabled");
    else
      fjputs_verbose("Certificate Transparency IS NOT enabled"); */

    const SSL_CIPHER *ciph = SSL_get_current_cipher(assl);

    if(!ciph)
      return error_tls(assl, 0, "SSL_get_current_cipher");

    tcon->cipher_name = SSL_CIPHER_get_name(ciph);

    fjprintf_verbose("Cipher Name: %s", tcon->cipher_name);

    tcon->cipher_version = SSL_CIPHER_get_version(ciph);

    fjprintf_verbose("Cipher Version: %s", tcon->cipher_version);

    char cbuf[BUFSIZ];

    tcon->cipher_description = SSL_CIPHER_description(ciph, cbuf, sizeof cbuf);

    if(tcon->cipher_description)
      fjputs_verbose(tcon->cipher_description);

    int abit = -1;

    tcon->cipher_bits = SSL_CIPHER_get_bits(ciph, &abit);

    if(tcon->cipher_bits > 0) 
      fjprintf_verbose("Cipher Bits: %d", tcon->cipher_bits);

    if(abit > 0)
      fjprintf_verbose("Cipher Processed: %d", abit);

    auto unsigned char buf[SHA1SIZ] = { 0x00 };
    const EVP_MD *digest = EVP_sha1();
    unsigned int len = 0;

    int rc = X509_digest(peer, digest, buf, &len);

    if (!rc || len != SHA1SIZ)
      tcon->certificate_fingerprint = "";

    char strbuf[2 * SHA1SIZ + 1] = { 0x00 };

    encode_hex(buf, strbuf, SHA1SIZ);

    tcon->certificate_fingerprint = strbuf;
    tcon->certificate_version = ((int) X509_get_version(peer)) + 1;
    tcon->certificate_serialnumber = create_serial(peer);

    const char *idhint = SSL_get_psk_identity_hint(assl);

    if(idhint && *idhint)
      fjprintf_verbose("PSK Identity Hint: %s\n", idhint);

    const char *identi = SSL_get_psk_identity(assl);

    if(identi && *identi)
      fjprintf_verbose("PSK Identity: %s\n", identi);

    const char *rstate = SSL_rstate_string_long(assl);
    
    if(rstate)
      fjprintf_verbose("SSL read state: %s\n", rstate);

    STACK_OF(X509_NAME) *astack = SSL_get_client_CA_list(assl);

    if(astack) {
      if(vcmd->verbose) {
        fputs("Not implemented\n", stdout);
      }
    }

    if(0) { /* renegotiate */
      ares = SSL_renegotiate(assl);

      if(ares <= 0)
        return error_tls(assl, ares, "SSL_renegotiate");

      ares = SSL_do_handshake(assl);

      if(ares <= 0)
        return error_tls(assl, ares, "SSL_do_handshake");

      if(SSL_renegotiate_pending(assl))
        fjputs_verbose("SSL renegotiation is pending!");
      else
        fjputs_verbose("SSL renegotiation is finished!");
    }
  }

  if(vcmd->verbose) { /* Just the certificate */
    X509_NAME_oneline(X509_get_subject_name(peer), abuf, sizeof abuf);

    fjprintf_verbose("Subject: %s", abuf);

    X509_NAME_oneline(X509_get_issuer_name(peer), abuf, sizeof abuf);

    fjprintf_verbose("Issuer: %s", abuf);
  } else if(vcmd->debug) { /* The whole chain */
    STACK_OF(X509) *x509 = SSL_get_peer_cert_chain(assl);

    for(register int i = 0;i < sk_X509_num(x509);++i) {
      X509_NAME_oneline(X509_get_subject_name(sk_X509_value(x509, i)), abuf, sizeof abuf);
      fjprintf_debug("Subject(%2d):%s", i, abuf);
      X509_NAME_oneline(X509_get_issuer_name(sk_X509_value(x509, i)), abuf, sizeof abuf);
      fjprintf_debug(" Issuer(%2d):%s", i, abuf);
    }
  } 

  if(vcmd->verbose) {
    output_x509nm(LN_commonName, xnam, NID_commonName);
    output_x509nm(LN_countryName, xnam, NID_countryName);
    output_x509nm(LN_localityName, xnam, NID_localityName);
    output_x509nm(LN_stateOrProvinceName, xnam, NID_stateOrProvinceName);
    output_x509nm(LN_organizationName, xnam, NID_organizationName);
    output_x509nm(LN_organizationalUnitName, xnam, NID_organizationalUnitName);
    
    long number = SSL_CTX_sess_number(actx);

    fjprintf_verbose("Number of sessions added to the cache: %ld", number);
  } 

  return aweb;
}
