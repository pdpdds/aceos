/*
GSH - Generic File System
	1) It is a Interface between Generic Storage Handler and File System Handler.
	2) To provide a straight forward approach to the application and other system
	   softwares this layer is included.

Created on 21-Apr-2002 By Sam
			     samuelhard@yahoo.com   www.geocities.com/samuelhard

*/

#ifndef __GENERIC_FS_H
#define __GENERIC_FS_H
#include <ExtKWord.h>
#include <ECollect.h>
#include <Str.h>

struct FindFile_Para
	{
	BYTE   Attribute;        /* attribute found */
	UINT16 CreationTime;     /* file time */
	UINT16 CreationDate;     /* file date */
	UINT16 AccessTime;
	UINT16 AccessDate;
	UINT32 FileSize;         /* file size */
	char   FileName[13];     /* found file name */

	BYTE   Reserved[20];
	};
struct __FSHandler
	{
	UINT32 (*GetDiskTotal)(char * Drive);
	UINT32 (*GetDiskFree)(char * Drive);
	UINT32 (*GetDiskBad)(char * Drive);

	FindFirst(char * FilePath,UINT16 Attribute, FindFile_Para * Result);
	FindNext(FindFile_Para * Result);

	/* Only this function uses the File Path to open a file. This function returns a
	unique id which must be used by other functions to access the opened file.
	This unique id is implemented because if specify the file path again and again you 
	have search for the sector again and again in this case you can preserve the current sector
	for the opened file.
	*/
	UINT16 (*Open)  (char * FilePath, BYTE OpenMode, BYTE ShareMode);
	UINT16 (*Close) (UINT16 FileHandle);
	UINT32 (*Seek)  (UINT16 FileHandle, UINT16 NewPos, BYTE From);
	UINT32 (*Tell)  (UINT16 FileHandle);
	UINT16 (*Read)  (UINT16 FileHandle, UINT16 NoOfBytes, void * Buffer);
	UINT16 (*Write) (UINT16 FileHandle, UINT16 NoOfBytes, void * Buffer);

	void * SpecificHandler; //pointer to a object
	};
struct __FSHandlerID
	{BYTE FSType;
	char Description[20];
	void * Handler;
	};

/*There must be one and only one object for this class. If it is not present
then, no program can access the File System. If duplicate(s) present, each have same
FS specifications and therefore wasting memory and may also cause dead lock */

enum GFS_ERROR_CODES
	{GFS_NO_ERROR,
	GFS_HANDLER_NOT_INSTALLED,
	};

class __GFS
	{
	private:
		Collection <__FSHandlerID *> Handlers;
		UINT16 LastErrorCode;
		BYTE NextDrive;
		BYTE NextFloppy;
	public:
		__GFS()
			{LastErrorCode=GFS_NO_ERROR;
			};
		~__GFS()
			{Handlers.Clear();
			}
		/*
		1) You must register a specific handler after initializing that handler
		2) This call must be done by FS drivers and the kernel.
		Input : - FSType     - Must be Unique
			  Desciption - A Descriptive Message about the FS
				       Must be maxiumum of 10 characters long
				       if shorter pad with space
			  Handler    - void pointer pointing the FS class
				       this class must implement GH class.
		Output : -
			  True - on success.    False - on failure.
		*/
		Boolean Register(const BYTE FSType,const char * Description, void * Handler);

		UINT32 GetDiskTotal(char Drive);
		UINT32 GetDiskFree (char Drive);
		UINT32 GetDiskBad  (char Drive);

		FindFirst(char * FilePath,UINT16 Attribute, void * ResultBuffer);
		FindNext();

		/* Share mode is not implemented*/
		UINT16 Open  (char * FilePath, BYTE OpenMode, BYTE ShareMode);
		UINT16 Close (UINT16 FileHandle);
		UINT32 Seek  (UINT16 FileHandle, UINT16 NewPos, BYTE From);
		UINT32 Tell  (UINT16 FileHandle);
		UINT16 Read  (UINT16 FileHandle, UINT16 NoOfBytes, void * Buffer);
		UINT16 Write (UINT16 FileHandle, UINT16 NoOfBytes, void * Buffer);

		void * SpecificHandler; //pointer to a object


		/* Returns the last error code. It contains the error occured
		at the GFS Level for extract error code for a particular FS
		use the FS handler */
		UINT32 GetLastErrorCode()
			{return LastErrorCode;
			}
		/* This function returns the string describing the ErrorCode*/
		char * GetErrorDescription(int ErrCode);
		char * GetErrorDescription()
			{return GetErrorDescription(LastErrorCode);
			}
		}GenericFS;

Boolean __GFS::Register(const BYTE FSType,const char * Description, void * Handler)
	{__FSHandlerID * NewHandler=new __FSHandlerID;
	char Key[2]={0,0};
	Key[0]=FSType;

	NewHandler->FSType=FSType;
	StrNCpy(NewHandler->Description,Description,20);
	NewHandler->Description[20]=0;
	NewHandler->Handler=Handler;

	return Handlers.Add(NewHandler,Key);
	}
char * __GFS::GetErrorDescription(int ErrCode)
	{switch ( ErrCode)
		{case GFS_NO_ERROR:
			return "No Error";
		case GFS_HANDLER_NOT_INSTALLED:
			return "Handler Not Installed";
		default:
			return "Unknown Error";
		}
	}


#endif
