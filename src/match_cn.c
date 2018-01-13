#include"fjorge.h"

HostnameValidationResult match_cn(const char *hostname, const X509 *servcert) {
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
