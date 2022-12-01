/*
    Shell for Ace OS
        Created on 10-Aug-2005 11:00PM by Samuel (samueldotj@gmail.com)
*/

#include "Shell.h"

COMMAND Commands[] =
{
    {"","",0,NULL},
    {"VER", "Displays version of the shell",  COMMAND_ID_VERSION, Version},
    {"HELP", "Shows helps about the available commands", COMMAND_ID_HELP, Help},
    {"DATE", "Shows current system time", COMMAND_ID_DATETIME, DateTime},
    {"QUIT", "Quits the shell", COMMAND_ID_QUIT, Quit},
    
    {"PWD", "Prints the working directory name", COMMAND_ID_PRINT_CURRENT_DIRECTORY, PrintCurrentWorkingDirectory},
    {"CD", "Changes current directory", COMMAND_ID_CHANGE_CURRENT_DIRECTORY, ChangeCurrentWorkingDirectory},
    {"MD", "Create new directory", COMMAND_ID_MAKE_DIRECTORY, MakeDirectory},
    {"RD", "Removes a directory", COMMAND_ID_REMOVE_DIRECTORY, RemoveDirectory},
    
    {"DIR", "Lists the files and folders", COMMAND_ID_LIST_FILES, ListFiles},
    {"DEL", "Delete files", COMMAND_ID_DELETE_FILES, DeleteFiles},
    {"REN", "Rename a file", COMMAND_ID_RENAME_FILE, RenameFile},
    {"TYPE", "Shows contents of a file", COMMAND_ID_TYPE, TypeFile},
    
    {"PROCESSES", "Displays the running process information", COMMAND_ID_LIST_PROCESS, ListProcess},
    {"MEM", "Shows current usage of memory", COMMAND_ID_LIST_MEMORY, ListMemory},
    
    {"SET", "Sets or displays environment variables", COMMAND_ID_SET, Set}
    
};

int main()
{
    //print system information
    printf("\n\rAce Shell %d.%d", MAJOR_VERSION, MINOR_VERSION);
    
    //initialize variables
    putenv("PROMPT=[$P$_]");
    
    //get command
    while(1)
    {
        printf("\n\r%s", GetPrompt() );
        int iCommandID = ReadCommand();
        switch ( iCommandID )
        {
            case 0:
                printf("\n\r Bad command or executable not found");
                break;
            case COMMAND_ID_QUIT:
                return 0;//normal exit
        }
    }
}
char szPrompt[MAX_PROMPT_SIZE];
char * GetPrompt()
{
    char * szPromptEnv = getenv("PROMPT");
    char ** szPromptVector;
    int i, iCount;
    if ( szPromptEnv == NULL )
        return "$";
    szPromptVector = SplitArgumentsIntoArray(szPromptEnv, &iCount, '$');
    if ( iCount == 0 || szPromptVector == NULL )
        return "$";
    strcpy( szPrompt, "");
    for(i=0; i<iCount; i++)
    {
        int iCopyAfter=1;
        switch ( toupper(szPromptVector[i][0]) )
        {
            case 'P':
            {
                char * buf;
                long  size = MAX_PATH;
                buf = (char *)malloc(size);
                if ( (buf) != NULL && getcwd(buf, size) != NULL  )
                    strcat( szPrompt, buf );
                break;
            }
            case 'A':
                strcat( szPrompt, "&" );
                break;
            case 'B':
                strcat( szPrompt, "B" );
                break;
            case 'C':
                strcat( szPrompt, "(" );
                break;
            case 'F':
                strcat( szPrompt, ")" );
                break;
            case 'G':
                strcat( szPrompt, ">" );
                break;
            case 'L':
                strcat( szPrompt, "<" );
                break;
            case 'S':
                strcat( szPrompt, " " );
                break;
            case '_':
                strcat( szPrompt, "$" );
                break;
            default:
                iCopyAfter=0;
                strcat( szPrompt, szPromptVector[i] );
        }
        if ( iCopyAfter )
            strcat( szPrompt, &szPromptVector[i][1] );
    }
    return szPrompt;
}

int ReadCommand()
{
    int i=0;
    int IsEndOfInput = 0;
    char szInput[MAX_INPUT_CHARACTER];
    char szCommand[MAX_INPUT_CHARACTER];
    char * szCommandLineArguments; 
    szInput[0]=0;
    while( i < MAX_INPUT_CHARACTER-1 && !IsEndOfInput)
    {
        scanf("%c", &szInput[i] );
        switch( szInput[i] )
        {
            case '\n':
            case '\r':
                i--;
                IsEndOfInput = 1;
                break;
            case '\b':
                //printf("\b");
                i-=2;
                if ( i < 0 )
                    i = 0;
                break;
            //default:
                //printf("%c", szInput[i] );
        }
        
        i++;
    }
    if ( i == 0 )
        return -1;
    
    szInput[i]=0;
    RTrim( LTrim(szInput ) );
    szCommandLineArguments = strchr( szInput , ' ');
    if ( szCommandLineArguments == NULL )
    {
        strcpy( szCommand, szInput);
    }
    else
    {
        strncpy( szCommand, szInput, szCommandLineArguments - szInput);
        szCommand[szCommandLineArguments - szInput] = 0;
        szCommandLineArguments++;
    }
            
    for(i=1;i<=COMMAND_ID_LAST;i++)
    {
        if ( strcmpi( Commands[i].szCommand, szCommand ) == 0 )
        {
            int ArgCount;
            //split the arguments into a string array
            char ** ArgVector = SplitArgumentsIntoArray(szCommandLineArguments, &ArgCount, ' ');
            Commands[i].FnShell( ArgCount, ArgVector );
            //delete the variable
            DeleteOneDimensionalArray(ArgVector, ArgCount );
            return i;
        }
    }
    //printf("\n\r [%s]", szInput);
    return 0;
}

void Version(int ArgCount, char *ArgVector[])
{
    printf("\n\rAce Shell Version %d.%d", MAJOR_VERSION, MINOR_VERSION);
}
void Help(int ArgCount, char *ArgVector[])
{
    int i;
    for(i=1;i<COMMAND_ID_LAST;i++)
        printf( "\n\r%-10s - %s", Commands[i].szCommand, Commands[i].szDescription) ;
}
void Quit(int ArgCount, char *ArgVector[])
{
    //exit(0);
}
void ListProcess(int ArgCount, char *ArgVector[])
{
    
}
void ListMemory(int ArgCount, char *ArgVector[])
{
    
}
void DateTime(int ArgCount, char *ArgVector[])
{
    time_t result;
    result = time(NULL);
    printf("%s", asctime(localtime(&result)));
    return;
}
