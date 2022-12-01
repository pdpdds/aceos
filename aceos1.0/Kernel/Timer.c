/*              Timer Routines
                      Developed by Sam(samuelhard@yahoo.com) on 19-June-2002
                      
*/
#include "TypeDefs.h"
#include "MemMan.h"

struct __TimerInfo
       {
       UINT16 TimerID;          // it is a unique number in the list, you can get the next unique number from the __TimerNumber structure
       UINT16 Interval;         //
       UINT16 Counter;
       BOOLEAN Enabled;
       void (*CallBackFn)();
       struct __TimerInfo * Next;
       };

struct __TimerInfo * __TimerQueue;
/* The __TimerQueue is a pointer to a __TimerInfo node in a linked list
   it always point to the next timer to execute.
   __TimerQueue is initialized by the InitializeTimer() function
*/
struct __TimerInfo * __TimerKernel;
/* This is the first node in the __TimerQueue and must be executed along with the
timer queue all over the time
*/

/* the following structure is used to allocate the unique numbers to the list.
 The list will keep the both the allocated number and the maximum allocated number.
 Through __TimerNoRoot you can search for the available numbers.*/
struct __TimerNumber
       {UINT16 Number;
       struct __TimerNumber * Next;
       };
struct __TimerNumber __TimerNoRoot;

UINT16 InitializeTimer(UINT16 Interval, void (*Kernel)() )
       {
       __TimerKernel =
       __TimerKernel->CallBackFn = Kernel;
       __TimerKernel->Interval = Interval;
       __TimerKernel->Counter = Interval;
       __TimerKernel->Enabled = True;
       __TimerKernel->Next = NULL;
       __TimerQueue = __TimerKernel;

       }
UINT16 CreateTimer(UINT16 Interval, void (*CallBackFn)())
       {
       }
void DropTimer(UINT16 TimerID)
       {
       }
void ChangeTimerInfo(UINT16 TimerID, UINT16 Interval, void (*CallBackFn)(), BOOLEAN Enabled)
       {
       }
