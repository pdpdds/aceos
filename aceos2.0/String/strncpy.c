/*---------------------------------------------------------------------------*/
/* strncpy() - copy characters from one string to another                    */
/*---------------------------------------------------------------------------*/
#include <string.h>

char *strncpy(char *string1, const char *string2, size_t count)
{
  char *p = string1;
  while(count)
  {
    *p = *string2;
    if(*string2)
    {
     string2++;
    }
    p++;
    count--;
  }
  return(string1);
}

