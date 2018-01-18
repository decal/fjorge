#include"fjorge.h"

int mydata_index = 1;

int verify_callback(int preverify_ok, X509_STORE_CTX *ctx) {
  char buf[256] = { '\0' }; 

  const int depth = X509_STORE_CTX_get_error_depth(ctx);
  int err = X509_STORE_CTX_get_error(ctx);
  const X509 *cert = X509_STORE_CTX_get_current_cert(ctx);
  const X509_NAME *iname = cert ? X509_get_issuer_name(cert) : NULL;
  const X509_NAME *sname = cert ? X509_get_subject_name(cert) : NULL;

  printf("iname: %x\n", iname);
  printf("iname: %x\n", sname);

  // validate_hostname("localhost", cert); 

  fjprintf_callback("verify_callback (depth=%d)(preverify=%d)", depth, preverify_ok);

  cbprint_cnname("Issuer (cn)", iname);
  cbprint_cnname("Subject (cn)", sname);

  if(!depth) 
    cbprint_sanname("Subject (san)", cert);

  // error_callback(err, "verify_callback");

  if(!preverify_ok) {
    switch(err) {
      case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
        puts(" Error = X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY");

        break;
        
      case X509_V_ERR_CERT_UNTRUSTED:
        puts(" Error = X509_V_ERR_CERT_UNTRUSTED");

        break;
      case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
        puts(" Error = X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN");

        break;
      case X509_V_ERR_CERT_NOT_YET_VALID:
        puts(" Error = X509_V_ERR_CERT_NOT_YET_VALID");

        break;
      case X509_V_ERR_CERT_HAS_EXPIRED:
        puts(" Error = X509_V_ERR_CERT_HAS_EXPIRED");

        break;
      case X509_V_OK:
        puts(" Error = X509_V_OK");

        break;
      default:
        printf(" Error = %d\n", err);

        break;
    }
  }

  const SSL *ssl = X509_STORE_CTX_get_ex_data(ctx, SSL_get_ex_data_X509_STORE_CTX_idx());
  const mydata_t *mydata = SSL_get_ex_data(ssl, mydata_index); 

  X509_NAME_oneline(X509_get_subject_name(cert), buf, sizeof buf); 

  if(depth > mydata->verify_depth) {
   preverify_ok = 0;
   err = X509_V_ERR_CERT_CHAIN_TOO_LONG;

   X509_STORE_CTX_set_error(ctx, err);
  } 

  if(!preverify_ok) 
    fjprintf_callback("verify error:num=%d:%s:depth=%d:%s", err, X509_verify_cert_error_string(err), depth, buf);
  else if (mydata->verbose_mode) 
    fjprintf_callback("depth=%d:%s", depth, buf);
  
  if(!preverify_ok && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT)) {
    X509_NAME_oneline(X509_get_issuer_name(cert), buf, sizeof buf);

    fjprintf_callback("issuer=%s", buf);
  }

  if(mydata->always_continue)
    return 1; 
  
  return preverify_ok;
}
