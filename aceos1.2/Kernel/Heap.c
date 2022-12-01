/*
	Heap Memory Manager for Ace OS Kernel / Applications
			- Created by Samuel on 08-Aug-2002 (samuelhard@yahoo.com)
*/

#include "TypeDefs.h"
#include "MemMan.h"

#define PAGE_SIZE 4096
extern void DEBUG();
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
	{struct HeapMemLink * HeapTemp, * HeapFreePrev , * HeapModifyFree,* ReqAddress;
	UINT32 ReqSize=Size+sizeof(struct HeapMemLink);
	UINT32 LargeSizeAvl=0;
	
	//Printf("\n\r Debug::Heap::AllocateHeap %ld",Size);
	if ( HeapFreeList )
		{HeapTemp = HeapFreeList;
		while ( HeapTemp )
			{if ( HeapTemp->MemSize > LargeSizeAvl )
				{LargeSizeAvl=HeapTemp->MemSize ;
				if ( HeapTemp->MemSize > ReqSize )
					break;
				}
			HeapTemp=HeapTemp->Next;
			}
		}
	if ( LargeSizeAvl < ReqSize )	// Checking whether memory is available or we have to allocate it
		{UINT32 ReqPages, AllocatedSize;
		ReqPages = ReqSize / PAGE_SIZE ;
		if ( ReqSize % PAGE_SIZE !=0  || ReqPages==0 )
			ReqPages++;
		struct HeapMemLink * HeapFree=HeapFreeList;
		struct HeapMemLink * HeapNew =(struct HeapMemLink * )AllocateKernelPages(ReqPages);
		if ( HeapNew == NULL )
			{//Printf("\n\r HEAP :: Allocation Failed for %d pages", ReqPages);
			return 0;
			}
		AllocatedSize = (ReqPages * PAGE_SIZE)-sizeof(struct HeapMemLink); // each page is 4096kb 
		//Printf("###AllocateKernelPages(%d)->%X",ReqPages,HeapNew);
		HeapNew->Next=NULL;	
		HeapNew->MemSize = AllocatedSize;
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
		{Printf("\n\rHEAP :: Unexpected End in the free List heap");
		return 0;
		}
	FreeMemory-=ReqSize;		// decreasing available memory
	ReqAddress = HeapTemp;		// Storing the alloted memory address
	//managing free list
	//Printf("\n\r HeapTemp->MemSize %d ReqSize %d",HeapTemp->MemSize, ReqSize);
	if ( HeapTemp->MemSize > ReqSize ) // checking whether the required node additional free memory , if available then break the node into another other
		{ReqAddress = (struct HeapMemLink*)( (UINT32)HeapTemp + (HeapTemp->MemSize-ReqSize) );	//modifing the alloted free node
		//Printf("\n\r ReqAddress %X",ReqAddress);
		HeapTemp->MemSize=HeapTemp->MemSize-ReqSize;
		}
	else	//directly linking the previous and next elements
		{
		if ( HeapTemp->Previous == NULL )
			HeapFreeList=HeapTemp->Next;
		if ( HeapTemp->Previous != NULL )
			HeapTemp->Previous->Next=HeapTemp->Next;
		if ( HeapTemp->Next != NULL )
			HeapTemp->Next->Previous=HeapTemp->Previous;
		}
	//managing occupied list
	HeapTemp = HeapOccupiedList; //Occupied list
	ReqAddress->MemSize=Size;
	HeapOccupiedList->Next=NULL;
	if ( HeapTemp == NULL )	// check whether occupied list root is exist
		{HeapOccupiedList=ReqAddress;
		ReqAddress->Previous=NULL;
		}
	else				// create new occupied list
		{while( HeapTemp->Next )
			HeapTemp=HeapTemp->Next;
		ReqAddress->Previous=HeapTemp;		
		HeapTemp->Next=ReqAddress;
		}
	//Printf("-[%X]-",(UINT32)ReqAddress+sizeof(struct HeapMemLink));
	return (UINT32)ReqAddress+sizeof(struct HeapMemLink);
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
