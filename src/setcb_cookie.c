#include"fjorge.h"

void setcb_cookie(cookie_cb cbfunc) {
  assert(cbfunc);

  cbak->set_cookie = cbfunc;

  return;
}
