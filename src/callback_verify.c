#include"fjorge.h"

/** TODO: needs serious cleaning up.. isn't printing common name! */

typedef struct verify_options_st {
  int depth;
  int quiet;
  int error;
  int return_error;
} VERIFY_CB_ARGS;

static VERIFY_CB_ARGS verify_args = { 0, 0, X509_V_OK, 0 };

static void nodes_print(const char *name, STACK_OF(X509_POLICY_NODE) *nodes)
{
  BIO_printf(bioerr, "%s Policies:", name);

  if (nodes) {
    BIO_puts(bioerr, CRLF);

    for(register signed int i = 0; i < sk_X509_POLICY_NODE_num(nodes); i++) {
      X509_POLICY_NODE *node = sk_X509_POLICY_NODE_value(nodes, i);
      X509_POLICY_NODE_print(bioerr, node, 2);
    } 
  } else {
    BIO_puts(bioerr, " <empty>\n");
  }
}

static void policies_print(X509_STORE_CTX *ctx)
{
  X509_POLICY_TREE *tree = X509_STORE_CTX_get0_policy_tree(ctx);
  const int explicit_policy = X509_STORE_CTX_get_explicit_policy(ctx);

  BIO_printf(bioerr, "Require explicit Policy: %s\n", explicit_policy ? "True" : "False");

  nodes_print("Authority", X509_policy_tree_get0_policies(tree));
  nodes_print("User", X509_policy_tree_get0_user_policies(tree));
}

int callback_verify(int ok, X509_STORE_CTX *ctx)
{
  const X509 *xcert = X509_STORE_CTX_get_current_cert(ctx);
  const int error = X509_STORE_CTX_get_error(ctx);
  const int depth = X509_STORE_CTX_get_error_depth(ctx);

  if(!(verify_args.quiet && ok)) {
    BIO_printf(bioerr, "*-* depth=%d ", depth);

    if(vcmd->debug) {
      if(xcert) 
        X509_NAME_print_ex(bioerr, X509_get_subject_name(xcert), 0, XN_FLAG_ONELINE);
      else
        BIO_puts(bioerr, "<no cert>");
    
      BIO_puts(bioerr, CRLF); 
    }
  }

  if(!ok) {
    BIO_printf(bioerr, "*-* verify error:num=%d:%s\n", error, X509_verify_cert_error_string(error));

    if(verify_args.depth >= depth) {
      if(!verify_args.return_error)
        ok = 1;

      verify_args.error = error;
    } else {
      ok = 0;
      verify_args.error = X509_V_ERR_CERT_CHAIN_TOO_LONG;
    }
  }

  switch (error) {
    case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
      BIO_puts(bioerr, "issuer= ");
      X509_NAME_print_ex(bioerr, X509_get_issuer_name(xcert), 0, XN_FLAG_ONELINE);
      BIO_puts(bioerr, CRLF);

      break;
    case X509_V_ERR_CERT_NOT_YET_VALID:
    case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
      BIO_puts(bioerr, "notBefore=");
      ASN1_TIME_print(bioerr, X509_get0_notBefore(xcert)); 
      BIO_puts(bioerr, CRLF);

      break;
    case X509_V_ERR_CERT_HAS_EXPIRED:
    case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
      BIO_puts(bioerr, "notAfter=");
      ASN1_TIME_print(bioerr, X509_get0_notAfter(xcert));
      BIO_puts(bioerr, CRLF);

      break;
    case X509_V_ERR_NO_EXPLICIT_POLICY:
      if(!verify_args.quiet)
        policies_print(ctx);

      break;
  }

  if(!verify_args.quiet) {
    if(error == X509_V_OK && ok == 2)
      policies_print(ctx);

    if(ok)
      BIO_printf(bioerr, "*-* verify return:%d\n", ok);
  }

  return ok;
}
