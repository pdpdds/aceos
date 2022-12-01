/*
Ace OS Shell Source Code - Environment related operations
*/
#include <Shell.h>
void Set(int ArgCount, char *ArgVector[])
{
    if ( ArgCount == 0 )
    {
        DisplayEnvironmentVariables();
        return;
    }
    if ( ArgCount == 1 )
    {
        char * szValue = getenv( ArgVector[0] );
        if ( szValue )
            printf("%s=%s", ArgVector[0], szValue );
        else
            printf("[%s] not defined", ArgVector[0] );
        return;
    }
    if ( ArgCount != 3  || strcmp(ArgVector[1],"=")!=0  )
    {
        printf("Invalid syntax. Supported syntax - VARIABLE = VALUE");
        return;
    }
    //if ( setenv( ArgVector[0], ArgVector[2] ) != 0 )
    //PrintErrorMessage(errno);
    char * szBuffer;
    szBuffer = malloc( strlen(ArgVector[0]) + strlen(ArgVector[2]) + 2 );
    if ( szBuffer == NULL )
    {
        printf("Memory allocation failed");
        return;
    }
    strcpy( szBuffer, ArgVector[0]);
    strcat( szBuffer, "= ");
    strcat( szBuffer, ArgVector[2]);
    if ( putenv( szBuffer ) != 0 )
        PrintErrorMessage(errno);
    free(szBuffer);
    
}
void DisplayEnvironmentVariables()
{
    char ** e = environ;
    while ( e )
    {
        if ( e[0]==0 )
            break;
        printf("%s\n", *e );
        e++;
    }
}
void UnSet(int ArgCount, char *ArgVector[])
{
    VALIDATE_ARGUMENT_COUNT(1,1);
    //unsetenv( ArgVector[0] );
}