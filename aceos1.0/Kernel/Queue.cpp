/* 	   Queue Management
		Created by Sam
				samuelhard@yahoo.com
*/

#ifndef __QUEUE_H
#define __QUEUE_H
#include <Queue.h>

Queue::Queue()
	{Root=0;
	}
Queue::~Queue()
	{struct QueueDataContainer * D,*tmp;
	D=Root;
	while( D )
		{tmp=D;
		D=D->Next;
		delete tmp;
		}
	}

inline void * Queue::Get()
	{if ( IsEmpty() )
		return 0;
	else
		{void * x;
		struct QueueDataContainer * tmp=Root;
		x=Root->ActualData;
		Root=Root->Next;
		delete tmp;
		return x;
		}
	}
inline void * Queue::Top()
	{return IsEmpty()?0:Root->ActualData;
	}
void Queue::Put(void * Value)
	{struct QueueDataContainer * New,* Last;

	New=new (struct QueueDataContainer);
	New->Next=0;
	New->ActualData=Value;
	if ( Root == 0 )
		Root=New;
	else
		{Last=Root;
		while( Last->Next )
			Last=Last->Next;
		Last->Next=New;
		}
	}
int Queue::ItemCount()
	{int i=0;
	struct QueueDataContainer * D;
	D=Root;
	while( D )
		{D=D->Next;
		i++;
		}
	return i;
	}
inline BOOLEAN Queue::IsEmpty()
	{return Root==0?True:False;
	}
#endif