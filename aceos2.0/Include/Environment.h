/*
  Name: 		Process Enviroment
  Author: 			- Samuel (samueldotj@gmail.com)
  Description: 	This file contains neccessary routines to maintain Environment variables
  Date: 04-Jul-2005 21:41
*/

#ifndef __ENVIRONMENT__H
#define __ENVIRONMENT__H

#include <TypeDefs.h>

DWORD GetEnvironmentVariable(LPTASK lpTask, LPCTSTR lpName, LPTSTR lpBuffer, DWORD dwSize);
BOOLEAN SetEnvironmentVariable(LPTASK lpTask, LPCTSTR lpName,  LPCTSTR lpValue);

DWORD GetCurrentDirectory(LPTASK lpTask, DWORD dwBufferLength,  LPTSTR lpBuffer);
BOOLEAN SetCurrentDirectory(LPTASK lpTask, LPCTSTR lpPathName );

LPTSTR GetCommandLine();

#endif

