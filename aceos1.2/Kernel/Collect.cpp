/*Created By Samuel -  17-12-2000
	1) Verified and modified using template rather than void pointer on 19-Jul-2001 12:19_57am
	2) Modified to structured chains on 25-Aug-2001 2:48_52pm
	3) Modified to function on Ace OS (new, delete operators are removed) on 17-Aug-2002 11:32 pm
*/

#include <Collect.h>

template<class PtrType> UINT16 Collection<PtrType>::Count()
	{return ItemCount;
	}
template<class PtrType> BOOLEAN Collection<PtrType>::Add(PtrType ptr,int Index,char * Key)
	{CollectionData <PtrType> **ptrPrev,* ptrNext=0,*ptrNew;
	UINT16 Pos=0;
	if ( StrLen(Key) > 14 )
		Key[14]=0;
	ptrPrev=&this->_Item;		//this loop will find the position where we have to put the new node
	while( Pos<Index && ptrPrev )
		{ptrPrev=&((*ptrPrev)->Next);
		Pos++;
		}
	ptrNext=(*ptrPrev==0)?0:(*ptrPrev)->Next; 	// you must insert before this node so keep this node's address

	//--------------Allocate New node and Initializing its values
	ptrNew=AllocHeap( sizeof(ptrNew) );
	if ( ptrNew == NULL )
		return NULL;
	ptrNew->Next=0;
	ptrNew->ptr=0;
	MemSet(ptrNew->Key,0,15);
	//----------------------------------------------------------

	ptrNew->ptr=ptr;
	StrCpy(ptrNew->Key,Key);
	ptrNew->Next=ptrNext;	  // placeing any other old nodes after that one

	if ( *ptrPrev == 0 )
		 *ptrPrev=ptrNew;     // putting the new node in the correct place
	else
		 (*ptrPrev)->Next=ptrNew;     // putting the new node in the correct place

	ItemCount++;		 // Incrementing the total

	return True;
	}
template <class PtrType> BOOLEAN Collection<PtrType>::Add(PtrType ptr)
	{return Add(ptr,ItemCount);
	}
template <class PtrType> BOOLEAN Collection<PtrType>::Add(PtrType ptr,char * Key)
	{CollectionData <PtrType>* ptrCur=this->_Item;
	while( ptrCur )
		{if ( StrCmp(Key,ptrCur->Key) == 0 )
			return False;
		ptrCur=ptrCur->Next;
		}
	return Add(ptr,ItemCount,Key);
	}
/* ----------------Donot work correctly on some occasions-------------*/
template<class PtrType> BOOLEAN Collection<PtrType>::Remove(int Index)
	{CollectionData <PtrType>**ptrCur=&this->_Item,**ptrPrev=0;
	UINT16 i=0;
	while( *ptrCur && i<=Index)
		{if ( i == Index )
			{if ( ptrPrev == 0 )
				this->_Item=(*ptrCur)->Next;
			else
				(*ptrPrev)->Next=(*ptrCur)->Next;
			DeallocHeap(*ptrCur);
			ItemCount--;
			return True;
			}
		ptrPrev=&*ptrCur;
		ptrCur=&(*ptrCur)->Next;
		i++;
		}
	return False;
	}

template<class PtrType> void Collection<PtrType>::Clear()
	{CollectionData  <PtrType> * ptrCur;
	if ( ptrCur != NULL )
		ptrCur = this->_Item->Next;
	else
		ptrCur = NULL;
	while(ptrCur && this->_Item)
		{CollectionData  <PtrType>* ptrNext=ptrCur->Next;   //Storing the Next Value
		delete DeallocHeap(ptrCur);			      //Deleting pointer
		ptrCur=ptrNext;			      //Next
		}
	this->_Item=0;
	ItemCount=0;
	}
template<class PtrType> PtrType Collection<PtrType>::Item(int Index)
	{CollectionData <PtrType>* ptrCur=this->_Item;
	UINT16 i=0;
	while( ptrCur )
		{if ( i==Index )
			return ptrCur->ptr;
		ptrCur=ptrCur->Next;
		i++;
		}
	return 0;
	}
template<class PtrType> PtrType Collection<PtrType>::Item(char * Key)
	{CollectionData <PtrType>* ptrCur=this->_Item;
	while( ptrCur )
		{if ( StrCmp(Key,ptrCur->Key) == 0 )
			return ptrCur->ptr;
		ptrCur=ptrCur->Next;
		}
	return 0;
	}

template<class PtrType> PtrType Collection<PtrType>::operator()(int Index)
	{return Item(Index);
	}
template<class PtrType> PtrType Collection<PtrType>::operator()(char *Key)
	{return Item(Key);
	}

