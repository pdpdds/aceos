/*
  Name: 		API Entry Finder
  Author: 			Samuel
  Date: 19-Nov-02 15:12
	The api entries are currently maintained as simple linked list it must be
replaced by hash table as soon as possible
*/
#include "String.h"
#include "Heap.h"
struct APIStruct
	{char * strAPIName;		//function name
	void * ptrAPIFunction;		//function pointer
	struct APIStruct * Next;
	};
struct APIStruct * RegisteredAPIEntries=0;

void RegisterAPI(char * strAPIName,void * ptrAPIFunction)
	{struct APIStruct * NewAPIStruct;
	char * strName;
	Printf("\n\r Reg : %s %x",strAPIName,ptrAPIFunction);
	//allocating the required space
	NewAPIStruct =(struct APIStruct *) AllocHeap(sizeof(struct APIStruct));
	strName = (char *) AllocHeap(StrLen(strAPIName)+1);

	StrCpy( strName, strAPIName);

	//assiging the values
	NewAPIStruct->strAPIName = strName;
	NewAPIStruct->ptrAPIFunction = ptrAPIFunction;
	NewAPIStruct->Next = 0;	//terminating the linked list
	
	if ( RegisteredAPIEntries )//if root exist find  the last node and add to it
		{struct APIStruct * CurAPIStruct=RegisteredAPIEntries;
		while ( CurAPIStruct->Next )
			{if ( StrCmp(CurAPIStruct->strAPIName, strAPIName)==0 ) //already the function exists update the pointer value
				break;
			CurAPIStruct = CurAPIStruct->Next;
			}
		CurAPIStruct->Next = NewAPIStruct;
		}	
	else
		RegisteredAPIEntries = NewAPIStruct; // creating node
	}
void * GetAPIEntryPoint(char * strAPIName)
	{struct APIStruct * CurAPIStruct=RegisteredAPIEntries;
	while ( CurAPIStruct )
		{Printf("\n\r [%s] [%s]", strAPIName, CurAPIStruct->strAPIName);
		if ( StrCmp(CurAPIStruct->strAPIName, strAPIName)==0 )
			return CurAPIStruct->ptrAPIFunction;
		CurAPIStruct = CurAPIStruct->Next;
		}
	return 0;
	}
