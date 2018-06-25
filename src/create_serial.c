#include"fjorge.h"

char *create_serial(X509 *acrt) {
  char sernum[SERIAL_NUM_LEN] = { 0x00 };

  ASN1_INTEGER *serial = X509_get_serialNumber(acrt);
  BIGNUM *bn = ASN1_INTEGER_to_BN(serial, NULL);

  if(!bn) {
    fjputs_error("Unable to convert ASN1_INTEGER to BIGNUM!");

    return "";
  }

  char *tmp = BN_bn2dec(bn);

  if(!tmp) {
    fjputs_error("Unable to convert BIGNUM to decimal string!");
    BN_free(bn);

    return "";
  }

  if(strlen(tmp) >= SERIAL_NUM_LEN) {
    fjputs_error("Buffer length shorter than serial number!");
    BN_free(bn);
    OPENSSL_free(tmp);

    return "";
  }

  strncpy(sernum, tmp, SERIAL_NUM_LEN);
  BN_free(bn);
  OPENSSL_free(tmp);

  char *aret = strdup(sernum);

  if(!aret)
    exit_verbose("strdup", __FILE__, __LINE__);

  return aret;
}
