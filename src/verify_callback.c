#include"fjorge.h"

typedef struct {
  int verbose_mode;
  int verify_depth;
  int always_continue;
} mydata_t;

static int mydata_index;

int verify_callback(int preverify_ok, X509_STORE_CTX *ctx) {
  char    buf[256];

  X509 *err_cert = X509_STORE_CTX_get_current_cert(ctx);
  int err = X509_STORE_CTX_get_error(ctx);
  int depth = X509_STORE_CTX_get_error_depth(ctx);
  SSL *ssl = X509_STORE_CTX_get_ex_data(ctx, SSL_get_ex_data_X509_STORE_CTX_idx());
  mydata_t *mydata = SSL_get_ex_data(ssl, mydata_index);

  X509_NAME_oneline(X509_get_subject_name(err_cert), buf, sizeof buf);

  if (depth > mydata->verify_depth) {
   preverify_ok = 0;
   err = X509_V_ERR_CERT_CHAIN_TOO_LONG;

   X509_STORE_CTX_set_error(ctx, err);
  }

  if (!preverify_ok) 
    printf("verify error:num=%d:%s:depth=%d:%s\n", err, X509_verify_cert_error_string(err), depth, buf);
  else if (mydata->verbose_mode) 
    printf("depth=%d:%s\n", depth, buf);
  
  if(!preverify_ok && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT)) {
    X509_NAME_oneline(X509_get_issuer_name(err_cert), buf, 256);

    printf("issuer= %s\n", buf);
  }

  if(mydata->always_continue)
    return 1;
  
  return preverify_ok;
}
