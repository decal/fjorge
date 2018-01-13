#include"fjorge.h"

HostnameValidationResult match_san(const char *hostname, const X509 *servcert) {
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

  sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);

  return result;
}
