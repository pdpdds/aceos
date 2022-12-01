/*              V86 Monitor for Ace OS
                      Developed by Sam(samueldotj@gmail.com) on 6-July-2004
                      The current system will support only one V86 task.
                      
*/
#define _DEBUG_

#include <TypeDefs.h>
#include <Memory.h>
#include <malloc.h>
#include <stdio.h>
#include <TaskMan.h>
#include <Thread.h>
#include <V86.h>
#include <Scheduler.h>
#include <Timer.h>
#include <Util.h>

#define VM_FLAG             0x20000
#define VALID_FLAGS         0xDFF

//Handles the GP for V86 task
//Ref : Mobius OS Code
BYTE sys_v86_IF_FLAG;

UINT32 V86GPHandler(UINT32 ECX, UINT32 EDX, LPV86TASKINFO pV86State, UINT32 EAX )
{
    BYTE * pV86Code;
    UINT32 * pIVT;
    WORD * pStack;
    DWORD * pStack32;
    BYTE bOperand32, bAddress32;
    
    DEBUG_PRINT_OBJECT3("CS %X EIP %X EFLags %X", pV86State->CS, pV86State->EIP, pV86State->EFlags);
    DEBUG_PRINT_OBJECT2("SS %X ESP %X", pV86State->SS, pV86State->ESP);
    
    pV86Code= (BYTE *) ((pV86State->CS<<4) + pV86State->EIP); //address of fault location
    pIVT = 0;          //IVT
    pStack = (WORD *) ((pV86State->SS<<4) + pV86State->ESP);
    pStack32 = (DWORD *)pStack;
    bOperand32=bAddress32=0;

    DEBUG_PRINT_OBJECT2("Code %X at %X", *pV86Code, pV86Code );
    while(1)
    {
        switch( *pV86Code )
        {
            case 0x66:
                DEBUG_PRINT_INFO("Operand 32");
                bOperand32 = 1;
                pV86State->EIP+=1;
                pV86Code++;
                break;
            case 0x67:
                DEBUG_PRINT_INFO("Data");
                bAddress32 = 1;
                pV86State->EIP+=1;
                pV86Code++;
                break;
            
            case 0x9C:      //pushf
                DEBUG_PRINT_INFO("PUSHF");
                if ( bOperand32 )
                {
                    pV86State->ESP = ( ( pV86State->ESP & 0xFFFF ) - 4 ) & 0xFFFF;
                    pStack32--;
                    pStack32[0] = pV86State->EFlags & VALID_FLAGS;
                    if ( sys_v86_IF_FLAG )
                        pStack32[0] |= V86_EFLAG_IF;
                    else
                        pStack32[0] &= ~V86_EFLAG_IF;
                }
                else
                {
                    pV86State->ESP = ( ( pV86State->ESP & 0xFFFF ) - 2 ) & 0xFFFF;
                    pStack--;
                    pStack[0] = pV86State->EFlags & VALID_FLAGS;
                    if ( sys_v86_IF_FLAG )
                        pStack[0] |= V86_EFLAG_IF;
                    else
                        pStack[0] &= ~V86_EFLAG_IF;
                }
                pV86State->EIP=pV86State->EIP+1;
                
                DEBUG_PRINT_OBJECT3("CS %X EIP %X EFLags %X", pV86State->CS, pV86State->EIP, pV86State->EFlags);
                DEBUG_PRINT_OBJECT2("SS %X ESP %X", pV86State->SS, pV86State->ESP);
                DEBUG_PRINT_INFO("Return 1");
                return 1;
            case 0x9D:
                DEBUG_PRINT_INFO("POPF");
                if ( bOperand32 )
                {
                    pV86State->EFlags = V86_EFLAG_IF | VM_FLAG | ( pStack32[0] & VALID_FLAGS ) ;
                    sys_v86_IF_FLAG = ( pStack32[0] & V86_EFLAG_IF ) != 0;
                    pV86State->ESP = ( ( pV86State->ESP & 0xFFFF ) + 4 ) & 0xFFFF;
                }
                else
                {
                    pV86State->EFlags = V86_EFLAG_IF | VM_FLAG | ( pStack[0] & VALID_FLAGS ) ;
                    sys_v86_IF_FLAG = ( pStack[0] & V86_EFLAG_IF ) != 0;
                    pV86State->ESP = ( ( pV86State->ESP & 0xFFFF ) + 2 ) & 0xFFFF;
                }
                pV86State->EIP=pV86State->EIP+1;
                DEBUG_PRINT_OBJECT3("CS %X EIP %X EFLags %X", pV86State->CS, pV86State->EIP, pV86State->EFlags);
                DEBUG_PRINT_OBJECT2("SS %X ESP %X", pV86State->SS, pV86State->ESP);
                DEBUG_PRINT_INFO("Return 1");
                return 1;
            case 0xCD:      //int
            {
                BYTE bIntNo= *(pV86Code+1);
                DEBUG_PRINT_OBJECT1("INT %X", bIntNo);
                DEBUG_PRINT_OBJECT3("IVT %X CS=%X EIP=%X",pIVT[bIntNo], pIVT[bIntNo]>>16, pIVT[bIntNo]&0x0000FFFF);
            
                pStack-=3;
                pV86State->ESP=((pV86State->ESP & 0xFFFF )-6) & 0xFFFF;
                //pStack = (pV86State->SS<<4)+pV86State->ESP;
            
                pStack[0] = pV86State->EIP+2;
                pStack[1] = pV86State->CS;
                pStack[2] = pV86State->EFlags;
                
                if ( sys_v86_IF_FLAG )
                    pStack[2] |= V86_EFLAG_IF;
                else
                    pStack[2] &= ~V86_EFLAG_IF;
                
                pV86State->CS = pIVT[bIntNo] >> 16;
                pV86State->EIP = pIVT[bIntNo] & 0x0000FFFF;
            }
                
                return 1;
            case 0xCF:      //iret
                DEBUG_PRINT_INFO("IRET");
                
                //pStack = (pV86State->SS<<4)+pV86State->ESP;
            
                pV86State->EIP = pStack[0];    
                pV86State->CS = pStack[1];
                pV86State->EFlags = pStack[2] | VM_FLAG | V86_EFLAG_IF;
            
                sys_v86_IF_FLAG = ( pStack[2] & V86_EFLAG_IF ) != 0;
            
                //pV86State->ESP += 6;
                pV86State->ESP = ( ( pV86State->ESP & 0xFFFF ) + 6 ) & 0xFFFF;
                
                return 1;
            case 0xFB:
                DEBUG_PRINT_INFO("STI");
                sys_v86_IF_FLAG = 1;
                pV86State->EIP=pV86State->EIP+1;
                return 1;
            case 0xFA:
                DEBUG_PRINT_INFO("CLI");
                sys_v86_IF_FLAG = 0;
                pV86State->EIP=pV86State->EIP+1;
                return 1;
            default:
                DEBUG_PRINT_OBJECT1("ILLEGAL INSTRUCTION %X", *pV86Code);
                asm("cli; hlt" );
                return 0;
        }
    }
    pV86Code=(BYTE *) ((pV86State->CS<<4)+ pV86State->EIP); 
    return 0;
}
