//Malloc
#include <TypeDefs.h>
#include <Heap.h>
#include <TaskMan.h>

void * malloc(size_t sizeObject)
{
    LPTASK lpTask=GetTaskInfo(GetTaskID());
    if ( lpTask == NULL )
        return NULL;
    return HeapAlloc( lpTask->lpHeapInfo, sizeObject);
}
void * free(void * pObject)
{
    LPTASK lpTask=GetTaskInfo(GetTaskID());
    if ( lpTask == NULL )
        return NULL;
    
    if ( HeapFree( lpTask->lpHeapInfo, pObject) )
        return pObject;
    else
        return NULL;
        
}

