#include"fjorge.h"

HEADER_LIST *add_header(const char *aline) {
  assert(aline);

  register PHEADER_LIST lsp = vcmd->request.hdrs;

  if(!lsp) {
    vcmd->request.hdrs = calloc(1, sizeof *(vcmd->request.hdrs));

    if(!vcmd->request.hdrs)
      exit_verbose("calloc", __FILE__, __LINE__);

    lsp = vcmd->request.hdrs;
    lsp->header = strdup(aline);

    if(!lsp->header)
      exit_verbose("strdup", __FILE__, __LINE__);
    
    return lsp;
  }

  while(lsp->next)
    lsp = lsp->next;
  
  lsp->next = calloc(1, sizeof *(lsp->next));

  if(!lsp->next)
    exit_verbose("calloc", __FILE__, __LINE__);

  lsp = lsp->next;
  lsp->header = strdup(aline);

  if(!lsp->header)
    exit_verbose("strdup", __FILE__, __LINE__);

  return lsp;
}
