/*
    Ace Shell File/Directory Related Operations
*/
#include <shell.h>
#include <dirent.h>

static inline unsigned char ToLower(unsigned char ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return ch + 32;
	else
		return ch;
}

int strcmpi(const char* s1, const char* s2)
	{
		const unsigned char* p1 = (const unsigned char*)s1;
		const unsigned char* p2 = (const unsigned char*)s2;
		int result;
		if (p1 == p2)
			return 0;
		while ((result = ToLower(*p1) - ToLower(*p2++)) == 0)
			if (*p1++ == '\0')
				break;
		return result;
	}

FILE * OpenFile(char * szFileName, char * szMode)
{
    FILE * fp;
    fp = (FILE *)fopen( szFileName, szMode );
    if ( fp == NULL )
    {
        printf("\nUnable to open the file [%s]", szFileName );
        PrintErrorMessage(errno);
        return NULL;
    }
    return fp;
}
void PrintCurrentWorkingDirectory(int ArgCount, char *ArgVector[])
{
    char * buf;
    long  size = MAX_PATH;
    buf = (char *)malloc(size);
    if ( (buf) != NULL && getcwd(buf, size) != NULL  )
        printf(" %s", buf);
    else
        PrintErrorMessage(errno);
    free( buf );
}
void ChangeCurrentWorkingDirectory(int ArgCount, char *ArgVector[])
{
    VALIDATE_ARGUMENT_COUNT(1,1);
    if ( chdir( ArgVector[0] ) != 0 )
        PrintErrorMessage(errno);
    
}
void MakeDirectory(int ArgCount, char *ArgVector[])
{
    VALIDATE_ARGUMENT_COUNT(1,1);
    if ( mkdir( ArgVector[0] ) != 0 )
        PrintErrorMessage(errno);
}
void RemoveDirectory(int ArgCount, char *ArgVector[])
{
    VALIDATE_ARGUMENT_COUNT(1,1);
    if ( rmdir( ArgVector[0] ) != 0 )
        PrintErrorMessage(errno);
}
void ListFiles(int ArgCount, char *ArgVector[])
{
    if ( ArgCount > 0 )
    {
        int  i=0;
        //loop through the arguments and print all directory contents
        for(i=0;i<ArgCount;i++)
            ShowDirectoryListingForDirectory( ArgVector[i] ) ;
    }
    //if no argument has given then display the current directory contents
    else
        ShowDirectoryListingForDirectory( "." );
}
void DeleteFiles(int ArgCount, char *ArgVector[])
{
    if ( ArgCount > 0 )
    {
        printf("Note - Current implementation does not supports wild cards");
        int  i=0, iDeletedFiles=0;
        //loop through the arguments and print all directory contents
        for(i=0;i<ArgCount;i++)
            iDeletedFiles += DeleteFile( ArgVector[i] );
        printf("\n %d file(s) deleted.", iDeletedFiles);
    }
    //if no argument has given then display the current directory contents
    else
        printf("Missing arguments");
}
void RenameFile(int ArgCount, char *ArgVector[])
{
    if ( ArgCount < 1 || ArgCount > 2 )
    {
        printf("Invalid number of arguments");
        return;
    }
    if ( rename(ArgVector[0], ArgVector[1] ) != 0 )
        PrintErrorMessage(errno);
}

int DeleteFile(char * szFileName)
{
    if ( remove( szFileName ) != 0 )
    {
        printf("\n%s : ", szFileName);
        PrintErrorMessage(errno);
        return 0;
    }
    return 1;
}
void ShowDirectoryListingForDirectory( char * szDirectory )
{
    DIR * dirp;
    struct dirent *dp;
    if ((dirp = opendir(szDirectory)) == NULL) 
    {
        PrintErrorMessage(errno);
        return;
    }
    printf("\nDirectory [%s] Contents --------------------------", szDirectory);
    do 
    {
        if ((dp = readdir(dirp)) != NULL) 
            printf("\n %-13s", dp->d_name);
    } while (dp != NULL);
    
    closedir(dirp);
}


void TypeFile(int ArgCount, char *ArgVector[])
{
    VALIDATE_ARGUMENT_COUNT(1, -1);
    int  i=0;
    for(i=0;i<ArgCount;i++)
        DisplayFile(  ArgVector[i] ) ;
}
void DisplayFile(char * szFileName)
{
    FILE * fp = (FILE *)OpenFile( szFileName, "r" );
    if ( fp == NULL )
        return;
    while ( !feof(fp) )
    {
        printf("%c", fgetc(fp) );
    }
    
}
