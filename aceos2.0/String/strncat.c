/*---------------------------------------------------------------------------*/
/* strncat() - concatenate characters to a string                            */
/*---------------------------------------------------------------------------*/
#include <string.h>

char *strncat(char *string1, const char *string2, size_t count)
{
  char *p;
  p = string1 + strlen(string1);
  while(*string2 && count)
  {
    *p = *string2;
    count--;
    string2++;
    p++;
  }
  *p = '\0';
  return(string1);
}

