#include"fjorge.h"

void info_callback(SSL *s, int where, int ret) {
  const char *str = NULL;
  const int w = where & ~SSL_ST_MASK;

  if (w & SSL_ST_CONNECT)
    str = "SSL_connect";
  else if (w & SSL_ST_ACCEPT)
    str = "SSL_accept";
  else
    str = "undefined";

  if (where & SSL_CB_LOOP) {
    printf("%s:%s\n", str, SSL_state_string_long(s));
  } else if (where & SSL_CB_ALERT) {
    str = (where & SSL_CB_READ) ? "read" : "write";
    
    printf("SSL3 alert %s:%s:%s\n", str, SSL_alert_type_string_long(ret), SSL_alert_desc_string_long(ret));
  } else if (where & SSL_CB_EXIT) {
    if (!ret) {
      printf("%s:failed in %s\n", str, SSL_state_string_long(s));
    } else if (ret < 0) {
      printf("%s:error in %s\n", str, SSL_state_string_long(s));
    }
 }

 return;
}
