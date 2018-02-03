#include"fjorge.h"

int callback_ocsp(SSL *s, void *arg)
{
  const unsigned char *p;
  OCSP_RESPONSE *rsp;
  int len = SSL_get_tlsext_status_ocsp_resp(s, &p);

  BIO_puts(arg, "OCSP response: ");

  if (!p) {
    BIO_puts(arg, "no response sent\n");

    return 1;
  }

  rsp = d2i_OCSP_RESPONSE(NULL, &p, len);

  if (!rsp) {
    BIO_puts(arg, "response parse error\n");
    BIO_dump_indent(arg, (char *)p, len, 4);

    return 0;
  }

  BIO_puts(arg, "\n======================================\n");

  OCSP_RESPONSE_print(arg, rsp, 0);

  BIO_puts(arg, "======================================\n");

  OCSP_RESPONSE_free(rsp);

  return 1;
}
