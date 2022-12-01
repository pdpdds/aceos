/*---------------------------------------------------------------------------*/
/* strcmp() - compare strings                                                */
/*---------------------------------------------------------------------------*/

int strcmp(const char *string1, const char *string2)
{
  int result = 0;
  while((*string1 == *string2) && (!result))
  {
    if(!(*string1) || !(*string2))
    {
      result = 1;
    } else {
      string1++;
      string2++;
    }
  }
  result = 0;
  if(*string1 != *string2)
  {
    result = 1;
    if(*string1 < *string2)
    {
      result = -1;
    }
  }
  return(result);
}

