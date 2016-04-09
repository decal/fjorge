#include"fjorge.h"

unsigned int add_header(char *restrict aline) {
  unsigned int ret = 1;
  PLINKED_LIST lsp = vcmd->request.hdrs;

  if(!vcmd->request.hdrs) {
    vcmd->request.hdrs = calloc(1, sizeof*(vcmd->request.hdrs));

    if(!vcmd->request.hdrs)
      error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

    lsp = vcmd->request.hdrs;
    lsp->header = strdup(aline);

    if(!lsp->header)
      error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));
    
    return ret;
  }

  while(lsp->next)
    lsp = lsp->next;
  
  lsp->next = calloc(1, sizeof*(lsp->next));

  if(!lsp->next)
    error_at_line(1, errno, __FILE__, __LINE__, "calloc: %s", strerror(errno));

  lsp = lsp->next;
  lsp->header = strdup(aline);

  if(!lsp->header)
    error_at_line(1, errno, __FILE__, __LINE__, "strdup: %s", strerror(errno));

  return ret;
}
