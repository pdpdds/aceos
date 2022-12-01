/*
  Name: 	API entry Finder for Ace OS
  Author: 		Samuel
  Date: 19-Nov-02 15:05
*/
#include "TypeDefs.h"

extern "C" void RegisterAPI(char * strAPIName,void * ptrAPIFunction);
//this function returns the API function pointer. 
extern "C" void * GetAPIEntryPoint(char * strAPIFunction);

