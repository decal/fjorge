#include"fjorge.h"

static BIO *bio_err = NULL;

typedef struct verify_options_st {
  int depth;
  int quiet;
  int error;
  int return_error;
} VERIFY_CB_ARGS;

static VERIFY_CB_ARGS verify_args = { 0, 0, X509_V_OK, 0 };

static void nodes_print(const char *name, STACK_OF(X509_POLICY_NODE) *nodes)
{
  BIO_printf(bio_err, "%s Policies:", name);

  if (nodes) {
    register int i;

    fputc('\n', stderr);

    for (i = 0; i < sk_X509_POLICY_NODE_num(nodes); i++) {
      X509_POLICY_NODE *node = sk_X509_POLICY_NODE_value(nodes, i);
      X509_POLICY_NODE_print(bio_err, node, 2);
    } 
  } else {
    fputs(" <empty>\n", stderr);
  }
}

static void policies_print(X509_STORE_CTX *ctx)
{
  X509_POLICY_TREE *tree = X509_STORE_CTX_get0_policy_tree(ctx);
  const int explicit_policy = X509_STORE_CTX_get_explicit_policy(ctx);

  fprintf(stderr, "Require explicit Policy: %s\n", explicit_policy ? "True" : "False");

  nodes_print("Authority", X509_policy_tree_get0_policies(tree));
  nodes_print("User", X509_policy_tree_get0_user_policies(tree));
}

int verify_callback(int ok, X509_STORE_CTX *ctx)
{
  bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);

  if(!bio_err)
    error_at_line(1, errno, __FILE__, __LINE__, "BIO_new_fp: %s", strerror(errno));

  const X509 *err_cert = X509_STORE_CTX_get_current_cert(ctx);
  const int err = X509_STORE_CTX_get_error(ctx);
  const int depth = X509_STORE_CTX_get_error_depth(ctx);

  if(!(verify_args.quiet && ok)) {
    fprintf(stderr, "depth=%d ", depth);

    if(err_cert) 
      X509_NAME_print_ex(bio_err, X509_get_subject_name(err_cert), 0, XN_FLAG_ONELINE);
    else
      fputs("<no cert>", stderr);

    fputc('\n', stderr); 
  }

  if(!ok) {
    fprintf(stderr, "verify error:num=%d:%s\n", err, X509_verify_cert_error_string(err));

    if(verify_args.depth >= depth) {
      if(!verify_args.return_error)
        ok = 1;

      verify_args.error = err;
    } else {
      ok = 0;
      verify_args.error = X509_V_ERR_CERT_CHAIN_TOO_LONG;
    }
  }

  switch (err) {
    case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
      fputs("issuer= ", stderr);
      X509_NAME_print_ex_fp(stderr, X509_get_issuer_name(err_cert), 0, XN_FLAG_ONELINE);
      fputc('\n', stderr);

      break;
    case X509_V_ERR_CERT_NOT_YET_VALID:
    case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
      fputs("notBefore=", stderr);
      ASN1_TIME_print(bio_err, X509_get0_notBefore(err_cert)); 
      fputc('\n', stderr);
      break;
    case X509_V_ERR_CERT_HAS_EXPIRED:
    case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
      fputs("notAfter=", stderr);
      ASN1_TIME_print(bio_err, X509_get0_notAfter(err_cert));
      fputc('\n', stderr);
      break;
    case X509_V_ERR_NO_EXPLICIT_POLICY:
      if(!verify_args.quiet)
        policies_print(ctx);

      break;
  }

  if(!verify_args.quiet) {
    if(err == X509_V_OK && ok == 2)
      policies_print(ctx);

    if(ok)
      fprintf(stderr, "verify return:%d\n", ok);
  }

  return ok;
}
