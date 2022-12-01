#ifndef __STACK__C
#define __STACK__C
/* Stack Management  */
#include <Stack.h>
Stack::Stack()
	{Root=0;
	}
Stack::~Stack()
	{struct StackDataContainer * D,*tmp;
	D=Root;
	while( D )
		{tmp=D;
		D=D->Back;
		delete tmp;
		}
	}

inline void * Stack::Pop()
	{if ( IsEmpty() )
		return 0;
	else
		{void * x;
		struct StackDataContainer * tmp=Root;
		x=Root->ActualData;
		Root=Root->Back;
		delete tmp;
		return x;
		}
	}
inline void * Stack::Top()
	{return IsEmpty()?0:Root->ActualData;
	}
inline void Stack::Push(void * Value)
	{struct StackDataContainer * New,* tmp;

	New=new (struct StackDataContainer);
	New->Back=0;
	New->ActualData=Value;
	tmp=Root;
	Root=New;
	Root->Back=tmp;
	}
int Stack::ItemCount()
	{int i=0;
	struct StackDataContainer * D;
	D=Root;
	while( D )
		{D=D->Back;
		i++;
		}
	return i;
	}
inline BOOLEAN Stack::IsEmpty()
	{return Root==0?True:False;
	}
#endif