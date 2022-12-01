/*
  Name: 		Process Enviroment
  Author: 			- Samuel (samueldotj@gmail.com)
  Description: 	This file contains neccessary routines to maintain Environment variables
  Date: 04-Jul-2005 21:41
*/
#include <TypeDefs.h>
#include <TaskMan.h>
#include <Environment.h>
#include <List.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <SysInfo.h>

DWORD GetEnvironmentVariable(LPTASK lpTask, LPCTSTR lpName, LPTSTR lpBuffer, DWORD dwSize)
{
    int i;
    char * szValue;
    
    if ( lpTask == NULL || lpBuffer == NULL )
        return 0;
    if ( lpTask->EnvironmentVariables == NULL )
        return 0;
    
    szValue = List_GetData(lpTask->EnvironmentVariables, lpName, -1);
    if ( szValue == NULL )
        return 0;
    for(i=0; szValue[i] != 0 && i<dwSize-1;i++)
        lpBuffer[i] = szValue[i];
    lpBuffer[i] = 0;
    
    //if buffer is too short return the required length
    if ( i >= dwSize-1 )
        while( lpTask->szCurrentDirectory[i] != 0 )
            i++;
    
    return i;
}

BOOLEAN SetEnvironmentVariable(LPTASK lpTask, LPCTSTR lpName,  LPCTSTR lpValue)
{
    if ( lpTask == NULL )
        return FALSE;
    if ( lpName == NULL )
        return FALSE;
    if ( lpName[0] == 0 )
        return FALSE;
    
    char * szValue = List_GetData(lpTask->EnvironmentVariables, lpName, -1);
    if ( szValue )
    {
        free(szValue);
        List_Delete(&lpTask->EnvironmentVariables, lpName, -1);
    }
    
    szValue = (char *)malloc( strlen(lpValue)+1 );
    strcpy( szValue, lpValue );
    
    List_Add( &lpTask->EnvironmentVariables, lpName, szValue);
    return TRUE;
}

DWORD GetCurrentDirectory(LPTASK lpTask, DWORD dwBufferLength,  LPTSTR lpBuffer)
{
    int i;
    if ( lpTask == NULL )
        return 0;
    if ( lpBuffer == NULL )
        return 0;
    for( i=0; lpTask->szCurrentDirectory[i] !=0 && i < dwBufferLength-1 ;i++ )
        lpBuffer[i] = lpTask->szCurrentDirectory[i];
    lpBuffer[i] = 0;
    
    //if buffer is too short return the required length
    if ( i >= dwBufferLength-1 )
        while( lpTask->szCurrentDirectory[i] != 0 )
            i++;
    
    return i;
    
}

BOOLEAN SetCurrentDirectory(LPTASK lpTask, LPCTSTR lpPathName )
{
    if ( lpTask == NULL )
        return FALSE;
    if ( lpPathName == NULL )
        return FALSE;
    if ( strlen(lpPathName) > CUR_DIR_SIZE )
        return FALSE;
    
    strcpy(lpTask->szCurrentDirectory,  lpPathName);
    return TRUE;
}

LPTSTR GetCommandLine()
{
    return "This version does not support Command Lines";
}

