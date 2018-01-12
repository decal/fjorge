#include"fjorge.h"

typedef struct {
  int verbose_mode;
  int verify_depth;
  int always_continue;
} mydata_t;

static int mydata_index;

typedef enum {
  MatchFound,
  MatchNotFound,
  NoSANPresent,
  MalformedCertificate,
  Error
} HostnameValidationResult;

static HostnameValidationResult matches_subject_alternative_name(const char *hostname, const X509 *servcert) {
  HostnameValidationResult result = MatchNotFound;
  register int i = -1, san_names_sb = -1;
  STACK_OF(GENERAL_NAME) *san_names = NULL;

  if(!(hostname && servcert))
    return Error;

  san_names = X509_get_ext_d2i((X509*) servcert, NID_subject_alt_name, NULL, NULL);

  if(!san_names)
    return NoSANPresent;

  san_names_sb = sk_GENERAL_NAME_num(san_names);

  for(i = 0;i < san_names_sb;++i) {
    const GENERAL_NAME *current_name = sk_GENERAL_NAME_value(san_names, i);

    if(current_name->type == GEN_DNS) {
      const char *dns_name = (char *)ASN1_STRING_get0_data(current_name->d.dNSName);

      if((size_t)ASN1_STRING_length(current_name->d.dNSName) != strlen(dns_name)) {
        result = MalformedCertificate;

        break;
      } else {
        printf("Subject Alternative Name: %s\n", dns_name);

        if(!strcmp(dns_name, hostname)) {
          result = MatchFound;

          break;
        }
      }
    }
  }

  sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAMES_free);

  return result;
}

static HostnameValidationResult matches_common_name(const char *hostname, const X509 *servcert) {
  const int common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name((X509 *)servcert), NID_commonName, -1);
  
  if(common_name_loc < 0)
    return Error;

  const X509_NAME_ENTRY *common_name_entry = X509_NAME_get_entry(X509_get_subject_name((X509 *)servcert), common_name_loc);

  if(!common_name_entry)
    return Error;

  const ASN1_STRING *common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);

  if(!common_name_asn1) 
    return Error;

  const char *common_name_str = (const char *)ASN1_STRING_get0_data(common_name_asn1);

  if(ASN1_STRING_length(common_name_asn1) != strlen(common_name_str))
    return MalformedCertificate;

  printf("Common Name: %s\n", common_name_str);

  if(strcasecmp(hostname, common_name_str))
    return MatchFound;

  return MatchNotFound;
}

static HostnameValidationResult validate_hostname(const char *hostname, const X509 *servcert) {
  if(!(hostname && servcert))
    return Error;

  register HostnameValidationResult result = matches_subject_alternative_name(hostname, servcert);

  if(result == NoSANPresent)
    result = matches_common_name(hostname, servcert);

  return result;
}

int verify_callback(int preverify_ok, X509_STORE_CTX *ctx) {
  char buf[256] = { '\0' }; 

  const int depth = X509_STORE_CTX_get_error_depth(ctx);
  int err = X509_STORE_CTX_get_error(ctx);
  const X509 *cert = X509_STORE_CTX_get_current_cert(ctx);
  const X509_NAME *iname = cert ? X509_get_issuer_name(cert) : NULL;
  const X509_NAME *sname = cert ? X509_get_subject_name(cert) : NULL;

  /* validate_hostname("localhost", cert); */

  fjprintf_callback("verify_callback (depth=%d)(preverify=%d)\n", depth, preverify_ok);

  cbprint_cnname("Issuer (cn)", iname);
  cbprint_cnname("Subject (cn)", sname);

  if(!depth) 
    cbprint_sanname("Subject (san)", cert);

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
