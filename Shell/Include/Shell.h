#ifndef _ACE_SHELL_H
#define _ACE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <dir.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define MAX_PATH 256

#define MAJOR_VERSION   0
#define MINOR_VERSION   1

#define MAX_INPUT_CHARACTER 256
#define MAX_PROMPT_SIZE 80
#define MAX_COMMAND_NAME 10


typedef struct
{
    char szCommand[MAX_COMMAND_NAME];
    char szDescription[50];
    int iCommandID;
    void (*FnShell)(int ArgCount, char *ArgVector[]);
}COMMAND;

enum COMMAND_ID
{
    COMMAND_ID_VERSION = 1,
    COMMAND_ID_HELP,
    COMMAND_ID_DATETIME,
    COMMAND_ID_QUIT,
    
    COMMAND_ID_PRINT_CURRENT_DIRECTORY,
    COMMAND_ID_CHANGE_CURRENT_DIRECTORY,
    COMMAND_ID_MAKE_DIRECTORY,
    COMMAND_ID_REMOVE_DIRECTORY,
    
    COMMAND_ID_LIST_FILES,
    COMMAND_ID_DELETE_FILES,
    COMMAND_ID_RENAME_FILE,
    COMMAND_ID_TYPE,
    
    COMMAND_ID_LIST_PROCESS,
    COMMAND_ID_LIST_MEMORY,

    COMMAND_ID_SET,
    COMMAND_ID_UNSET,
    
    COMMAND_ID_LAST
};

#define VALIDATE_ARGUMENT_COUNT( Min, Max )\
    if ( ArgCount < Min )\
    {\
        printf("Missing parameters");\
        return;\
    }\
    if ( ArgCount > Max && Max != -1 )\
    {\
        printf("Too many parameters");\
        return;\
    }\


//Shell Functions
void Version(int ArgCount, char *ArgVector[]);
void Help(int ArgCount, char *ArgVector[]);
void Quit(int ArgCount, char *ArgVector[]);
void DateTime(int ArgCount, char *ArgVector[]);

int ReadCommand();
char * GetPrompt();

//file related functions
void ListFiles(int ArgCount, char *ArgVector[]);
void ShowDirectoryListingForDirectory( char * szDirectory );
void DeleteFiles(int ArgCount, char *ArgVector[]);
int DeleteFile(char * szFileName);
void RenameFile(int ArgCount, char *ArgVector[]);

void TypeFile(int ArgCount, char *ArgVector[]);
void DisplayFile(char * szFileName);
FILE * OpenFile(char * szFileName, char * szMode);

void PrintCurrentWorkingDirectory(int ArgCount, char *ArgVector[]);
void ChangeCurrentWorkingDirectory(int ArgCount, char *ArgVector[]);
void MakeDirectory(int ArgCount, char *ArgVector[]);
void RemoveDirectory(int ArgCount, char *ArgVector[]);


void ListProcess(int ArgCount, char *ArgVector[]);
void ListMemory(int ArgCount, char *ArgVector[]);


//environment related functions
void Set(int ArgCount, char *ArgVector[]);
void DisplayEnvironmentVariables();
void UnSet(int ArgCount, char *ArgVector[]);


//utility functions
int GetTotalTokens(char * szSource, char ch);
char ** SplitArgumentsIntoArray(char * szArguments, int * iArgCount, char Separator);
void DeleteOneDimensionalArray(void * Array, int ArgCount );
char * LTrim(char * szSource);
char * RTrim(char * szSource);
void PrintErrorMessage(int iErrorNo);


#endif

