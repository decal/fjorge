#include"fjorge.h"

// NID_commonName
unsigned char *output_x509nm(const char *label, const X509_NAME *const name, const int anid) {
  assert(label);

  register int suc = 0;
  unsigned char *utf = NULL;

  do {
    if(!name)
      break;

    const int idx = X509_NAME_get_index_by_NID((X509_NAME *) name, anid, -1);

    if(idx <= -1) 
      break;

    const X509_NAME_ENTRY *entry = X509_NAME_get_entry((X509_NAME *) name, idx);

    if(!entry)
      break;

    const ASN1_STRING *data = X509_NAME_ENTRY_get_data((X509_NAME_ENTRY *) entry);

    if(!data)
      break;

    const int length = ASN1_STRING_to_UTF8(&utf, data);

    if(!(utf && length > 0))
      break;

    fjprintf_callback(" %s: %s", label, utf);

    suc = 1;
  } while(0);

  if(!suc)
    fjprintf_callback(" %s: <not available>", label);

  return utf;
}
