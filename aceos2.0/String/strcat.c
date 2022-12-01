/*---------------------------------------------------------------------------*/
/* strcat() - concatenate strings                                            */
/*---------------------------------------------------------------------------*/
#include <string.h>

char *strcat(char *string1, const char *string2)
{
  char *p= string1 + strlen(string1);
  while(*string2)
  {
    *p = *string2;
    string2++;
    p++;
  }
  *p = '\0';
  return(string1);
}

