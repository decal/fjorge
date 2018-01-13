#include"fjorge.h"

HostnameValidationResult validate_hostname(const char *hostname, const X509 *servcert) {
  if(!(hostname && servcert))
    return Error;

  register HostnameValidationResult result = match_san(hostname, servcert);

  if(result == NoSANPresent)
    result = match_san(hostname, servcert);

  return result;
}
