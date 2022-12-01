/*
  Name: 		Input / Output Routines
  Author: 			- Samuel
  Date: 29-Jul-03 4:47pm
        Converted to AT&T style assembly from the existing Intel Style Assembly file.
*/
#include <TypeDefs.h>
#include <stdio.h>

BYTE  InPortByte(WORD Port)
{
    BYTE Result;
    asm volatile("movw %1, %%dx;\
                 in %%dx, %%al;\
                 movb %%al, %0"
                :"=m"(Result)
                :"m"(Port)
                :"%eax","%edx");
    return Result;
}
WORD  InPortWord(WORD Port)
{
    WORD Result;
    asm volatile("movw %1, %%dx;\
                 in %%dx, %%ax;\
                 movw %%ax, %0"
                :"=m"(Result)
                :"m"(Port)
                :"%eax","%edx");
    return Result;

}
DWORD InPortDWord(WORD Port)
{
    DWORD Result;
    asm volatile("movw %1, %%dx;\
                 in %%dx, %%eax;\
                 movl %%eax, %0"
                :"=m"(Result)
                :"m"(Port)
                :"%eax","%edx");
    return Result;

}

void OutPortByte(WORD Port, BYTE Value)
{
    asm volatile("movw %0, %%dx;\
                  movb %1, %%al;\
                  out %%al, %%dx;"
                : 
                :"m"(Port),"m"(Value)
                :"%eax","%edx");
}
void OutPortWord(WORD Port, WORD Value)
{
    asm volatile("movw %0, %%dx;\
                  movw %1, %%ax;\
                  out %%ax, %%dx;"
                : 
                :"m"(Port),"m"(Value)
                :"%eax","%edx");
}
void OutPortDWord(WORD Port, DWORD Value)
{
    asm volatile("movw %0, %%dx;\
                  movl %1, %%eax;\
                  out %%eax, %%dx;"
                : 
                :"m"(Port),"m"(Value)
                :"%eax","%edx");
}

