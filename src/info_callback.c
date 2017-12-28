#include"fjorge.h"

void info_callback(const SSL *s, int where, int ret) {
  if(vcmd->debug > 1) {
    const char *str = "undefined";
    const int w = where & ~SSL_ST_MASK;

    if (w & SSL_ST_CONNECT)
      str = "SSL_connect";
    else if (w & SSL_ST_ACCEPT)
      str = "SSL_accept";

    if (where & SSL_CB_LOOP) {
      fprintf(stderr, "[C] %s:%s\n", str, SSL_state_string_long(s));
    } else if (where & SSL_CB_ALERT) {
      str = (where & SSL_CB_READ) ? "read" : "write";
   
      fprintf(stderr, "[C] SSL3 alert %s:%s:%s\n", str, SSL_alert_type_string_long(ret), SSL_alert_desc_string_long(ret));
    } else if (where & SSL_CB_EXIT) {
      if (!ret)
        fprintf(stderr, "[C] %s:failed in %s\n", str, SSL_state_string_long(s));
      else if (ret < 0)
        fprintf(stderr, "[C] %s:error in %s\n", str, SSL_state_string_long(s));
    }
  }

  return;
}
