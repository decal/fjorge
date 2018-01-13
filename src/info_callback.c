#include"fjorge.h"

void info_callback(const SSL *s, int where, int ret) {
  const char *str = "undefined";
  const int w = where & ~SSL_ST_MASK;

  if (w & SSL_ST_CONNECT)
    str = "SSL_connect";
  else if (w & SSL_ST_ACCEPT)
    str = "SSL_accept";

  if (where & SSL_CB_LOOP) {
    fjprintf_callback("%s:%s", str, SSL_state_string_long(s));
  } else if (where & SSL_CB_ALERT) {
    str = (where & SSL_CB_READ) ? "read" : "write";

    fjprintf_callback("SSL3 alert %s:%s:%s", str, SSL_alert_type_string_long(ret), SSL_alert_desc_string_long(ret));
  } else if (where & SSL_CB_EXIT) {
    if (!ret)
      fjprintf_callback("%s:failed in %s", str, SSL_state_string_long(s));
    else if (ret < 0)
      fjprintf_callback("%s:error in %s", str, SSL_state_string_long(s));
  }

  return;
}
