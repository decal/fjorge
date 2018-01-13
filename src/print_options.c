#include"fjorge.h"

void print_options(FILE *fptr) {
  assert(fptr);

  fputs("*** OPTIONS: headers = ", fptr);

  if(vcmd->request.hdrs) {
    register PLINKED_LIST lsp = vcmd->request.hdrs;

    do {
      fputs(lsp->header, fptr);

      lsp = lsp->next;

      if(lsp)
        fputc(',', fptr);
    } while(lsp);
  }

  fprintf(fptr, " namein = %s", vcmd->namein);
  fprintf(fptr, " input = %p", (const void *)vcmd->input);
  fprintf(fptr, " nameout = %s", vcmd->nameout);
  fprintf(fptr, " output = %p", (const void *)vcmd->output);
  fprintf(fptr, " namedir = %s", vcmd->namedir);
  fprintf(fptr, " dirfiles = %p", (const void *)vcmd->dirfiles);
  fprintf(fptr, " verbose = %u", vcmd->verbose);
  fprintf(fptr, " debug = %u", vcmd->debug);
  fprintf(fptr, " secure = %u", vcmd->secure);
  fprintf(fptr, " fuzz = %u", vcmd->fuzz);
  fprintf(fptr, " shuffle = %u", vcmd->shuffle);
  fprintf(fptr, " multiply = %u", vcmd->multiply);
  fprintf(fptr, " casing = %u", vcmd->casing);
  fprintf(fptr, " inject = %u", vcmd->inject);
  fprintf(fptr, " hostnam = %s", vcmd->hostnam);
  fprintf(fptr, " portnum = %d\n", vcmd->portnum);

  return;
}
