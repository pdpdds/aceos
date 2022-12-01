#include <TypeDefs.h>
#include <string.h>
#include <ctype.h>

int strnicmp(const char *string1, const char *string2, size_t count)
{
  int result = 0;
  while((toupper(*string1) == toupper(*string2)) && (count > 1))
  {
    count--;
    if(!(*string1) || !(*string2))
    {
      count = 0;
    } else {
      string1++;
      string2++;
    }
  }
  result = 0;
  if(toupper(*string1) > toupper(*string2))
  {
    result = 1;
  } else {
    if(toupper(*string1) < toupper(*string2))
    {
      result = -1;
    }
  }
  return(result);
}

