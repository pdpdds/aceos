/*
Ace Shell Utility Functions
*/
#define SPACE_CHARACTER ' '

#include <Shell.h>
//this function will split the given space separated tokens into dynamically allocated string array
//note - the callee should to take care deleting the alllocated array
char ** SplitArgumentsIntoArray(char * szArguments, int * iArgCount, char Separator)
{
    *iArgCount = 0;
    if ( szArguments == NULL || szArguments[0]==0 )
        return NULL;
    if ( strlen(LTrim(szArguments)) == 0 )
        return NULL;
    
    char * * ArgVector;
    char * sCurToken;
    int i=0, iCurToken=0;
    
    //skip the continues spaces
    while( szArguments[i] == Separator )
        i++;
    
    int iTotalTokens = GetTotalTokens( &szArguments[i], Separator);
    ArgVector = malloc( iTotalTokens * sizeof( char * ) );
    if ( ArgVector == NULL )
        return NULL;
    
    sCurToken = &szArguments[i];
    while( sCurToken )
    {
        int iTokenSize=0;
        
        //calculate token size
        char * szTokenEnd = strchr( sCurToken+1, Separator );
        if ( szTokenEnd )
            iTokenSize = szTokenEnd - sCurToken;
        else
            iTokenSize = strlen( szArguments ) - (sCurToken - szArguments);

        //dynamically allocate memory for the token
        ArgVector[iCurToken] = malloc( iTokenSize + 1 );
        if ( ArgVector[iCurToken]  == NULL )
        {
            printf("\n Error while parsing arguments - unable to allocate memory");
            return ArgVector;
        }
        //increase the total argument count
        //*(iArgCount)=*(iArgCount)+1;
        (*(iArgCount))++;
        
        //copy the token into allocated area
        strncpy( ArgVector[iCurToken], sCurToken, iTokenSize );
        ArgVector[iCurToken][iTokenSize]=0;
        
        iCurToken++;
        
        //find where the next token starts
        sCurToken = strchr( &szArguments[i+1], Separator );
        if ( sCurToken )
            i = sCurToken - szArguments;
        else
            i = strlen( szArguments);
        
        //skip the continues spaces
        while( szArguments[i] == Separator )
            i++;
        //if end of string break the loop
        if ( szArguments[i] == 0 )
            break;
        
        //find the token        
        sCurToken = &szArguments[i];
    }        
    return ArgVector;
}
int GetTotalTokens(char * szSource, char ch)
{
    int i, result=0;
    char PreviousCharacter=0;
    for(i=0;szSource[i];i++)
    {
        if ( szSource[i] == ch && PreviousCharacter != szSource[i])
            result++;
        PreviousCharacter = szSource[i];
    }
    return result + 1;
}
void DeleteOneDimensionalArray(void * Array, int ArgCount )
{
    int i=0;
    //first delete the array contents
    for( i=0; i<ArgCount; i++ )
        free( ((char **)Array)[i] );
    
    //delete the array itself
    free(Array);
}
char * LTrim(char * szSource)
{
    int i,j;
    if ( szSource[0] == 0 )
        return NULL;
    
    for(i=0; szSource[i] == SPACE_CHARACTER; i++);
    if ( i!=0 )
    {
        for(j=0; szSource[i]; i++,j++)
            szSource[j] = szSource[i];
        szSource[j]=0;
    }
    return szSource;
}
char * RTrim(char * szSource)
{
    int i;
    if ( szSource[0] == 0 )
        return NULL;
    
    for(i=strlen(szSource)-1; szSource[i] == SPACE_CHARACTER; i--);
    szSource[i+1]=0;
    return szSource;
}
void PrintErrorMessage(int iErrorNo)
{
    char * szError= strerror(iErrorNo);
    printf(" %s", szError );
}
