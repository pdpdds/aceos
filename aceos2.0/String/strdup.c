/*---------------------------------------------------------------------------*/
/* strdup() - create a duplicate string                                      */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>

char *strdup(const char *string)
{
  char *p;
  size_t slen;
  slen = strlen(string);
  p = malloc(slen + 1);
  strcpy(p, string);
  return(p);
}

