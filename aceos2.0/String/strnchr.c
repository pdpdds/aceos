#include <TypeDefs.h>
#include <string.h>
#include <ctype.h>

char *strnchr(const char *string, int c,size_t count)
{
  char *p = NULL;
  while(*string && count > 0)
  {
    if((int)(*string) == c)
    {
      p = (char *)string;
      break;
    }
    string++;
    count--;
  }
  return(p);
}
