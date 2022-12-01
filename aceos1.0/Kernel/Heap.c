/*
	Heap Memory Manager for Ace OS Kernel / Applications
			- Created by Samuel on 08-Aug-2002 (samuelhard@yahoo.com)
*/

#include "TypeDefs.h"
#include "MemMan.h"

#define PAGE_SIZE 4096

UINT32 FreeMemory=0;
struct HeapMemLink
	{UINT32 MemSize;
	struct HeapMemLink * Next;
	struct HeapMemLink * Previous;
	}__attribute__ ((packed));
struct HeapMemLink * HeapFreeList=NULL;
struct HeapMemLink * HeapOccupiedList=NULL;

/*Allocate Heap memory to the application*/
void * AllocHeap(UINT32 Size)
	{struct HeapMemLink * HeapTemp, * HeapFreePrev , * HeapModifyFree;
	UINT32 ReqSize=Size+sizeof(struct HeapMemLink);
	UINT32 LargeSizeAvl=0;
	void * ReqAddress;
	if ( HeapFreeList )
		{HeapTemp = HeapFreeList;
		while ( HeapTemp )
			{if ( HeapTemp->MemSize > LargeSizeAvl )
				{LargeSizeAvl=HeapTemp->MemSize ;
				break;
				}
			HeapTemp=HeapTemp->Next;
			}
		}
	if ( LargeSizeAvl < ReqSize )	// Checking whether memory is available or we have to allocate it
		{UINT32 ReqPages, AllocatedSize;
		ReqPages = (ReqSize+sizeof(struct HeapMemLink)) / PAGE_SIZE ;
		if ( (ReqSize+sizeof(struct HeapMemLink)) % PAGE_SIZE !=0  || ReqPages==0 )
			ReqPages++;
		AllocatedSize = (ReqPages * PAGE_SIZE); // each page is 4096kb 
		struct HeapMemLink * HeapFree=HeapFreeList;
		struct HeapMemLink * HeapNew =(struct HeapMemLink * )AllocatePages(ReqPages);
		HeapNew->Next=NULL;	
		HeapNew->MemSize = AllocatedSize;
		if ( HeapNew == NULL )
			{Printf("\n\r HEAP :: Allocation Failed for %d pages", ReqPages);
			return 0;
			}
		if ( HeapFree )				//checking for availablity of root node
			{while(HeapFree->Next)  	//moving to last node
				HeapFree=HeapFree->Next;
			HeapFree->Next=HeapNew;
			HeapNew->Previous=HeapFree;
			}
		else					//create root node
			{HeapFreeList=HeapNew;
			HeapNew->Previous=NULL;
			}
		FreeMemory+=AllocatedSize;
		}
	HeapTemp=HeapFreeList;
	while( HeapTemp )		//Searching for required memory
		{if ( HeapTemp->MemSize >= ReqSize )	// if memory available exit
			break;
		else
			HeapTemp=HeapTemp->Next;
		}
			
	if ( HeapTemp == NULL )		// if not available then exit
		return 0;
	FreeMemory-=ReqSize;		// decreasing available memory
	ReqAddress = HeapTemp;		// Storing the alloted memory address
	
	if ( HeapTemp->MemSize > ReqSize ) // checking whether the required node additional free memory , if available then break the node into another other
		{
		HeapModifyFree=(struct HeapMemLink*)((UINT32)HeapTemp+ReqSize);	//modifing the alloted free node
		HeapModifyFree->MemSize=HeapTemp->MemSize-ReqSize;

		HeapModifyFree->Next=HeapTemp->Next;
		HeapModifyFree->Previous=HeapTemp->Previous;

		if ( HeapTemp->Previous == NULL )
			HeapFreeList=HeapModifyFree;
		else
			HeapTemp->Previous->Next=HeapModifyFree;
		}
	else	//directly linking the previous and next elements
		{
		if ( HeapTemp->Previous == NULL )
			HeapFreeList=HeapTemp->Next;
		else
			HeapTemp->Previous->Next=HeapTemp->Next;
		if ( HeapTemp->Next != NULL )
			HeapTemp->Next->Previous=HeapTemp->Previous;
		}
	HeapTemp = HeapOccupiedList; //Occupied list
	if ( HeapTemp == NULL )
		{
		HeapOccupiedList=ReqAddress;
		HeapOccupiedList->MemSize=Size;
		HeapOccupiedList->Next=HeapOccupiedList->Previous=NULL;
		}
	else
		{struct HeapMemLink * HeapNewOccupied;
		while( HeapTemp->Next )
			HeapTemp=HeapTemp->Next;
		HeapNewOccupied=ReqAddress;
		HeapNewOccupied->MemSize=Size;
		HeapNewOccupied->Next=NULL;
		HeapNewOccupied->Previous=HeapTemp;		
		HeapTemp->Next=HeapNewOccupied;
		}
	return ReqAddress+sizeof(struct HeapMemLink);
	}
/*Deallocates the already allocated memory and retains the address for
future use*/
BYTE DeAllocHeap(UINT32 Address)
	{struct HeapMemLink * HeapTemp=HeapOccupiedList;
	struct HeapMemLink * HeapFree=HeapFreeList;
	while(HeapTemp)
		{
		if ( (UINT32)HeapTemp == Address )
			{struct HeapMemLink * HeapDel;   //deleting Node
			HeapDel = HeapTemp->Previous;
			HeapDel->Next = HeapTemp->Next;	
			FreeMemory+=HeapTemp->MemSize+sizeof(struct HeapMemLink);     // increasing memsize
			
			while(HeapFree->Next)  		 //inserting node
				HeapFree=HeapFree->Next;
			HeapFree->Next=HeapTemp;
			HeapTemp->Next=NULL;
			HeapTemp->Previous=HeapFree;
	
			return 1;				//that's all exit the loop
			}	
		HeapTemp=HeapTemp->Next;
		}
	return 0;
	}
