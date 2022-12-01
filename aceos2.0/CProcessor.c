/*
  Name: 		X86 Processor handling for Ace OS
  Author: 			- Sam (samueldotj@gmail.com)
  Date: 		17-Jun-2004 08:04
  
*/
#include <printf.h>
#include <processor.h>

#define TOTAL_EXCEPTIONS    30

CPUID_DATA sysCPUBasicInformation[CPUID_MAX_BASIC_LEVEL];
CPUID_DATA sysCPUExtendedInformation[CPUID_MAX_EXTENDED_LEVEL];   

char DumpCPUFormat[]="\n\r----------------------------- EXCEPTION HANDLER -----------------------------\
\n\rEAX=%X EBX=%X ECX=%X EDX=%X\
\n\rCS=%X DS=%X ES=%X SS=%X GS=%X FS=%X\
\n\rEBP=%X ESP=%X ESI=%X EDI=%X EIP=%X\
\n\rCR0=%X CR1=%X CR2=%X CR3=%X CR4=%X\
\n\rEFlags=%X Error Code=%X\
\n\rException Type :: %s (%d)\
\n\r-----------------------------------------------------------------------------";

char szExceptionType[TOTAL_EXCEPTIONS][50]=
{
    "Divide by zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalide opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Floating point error",
    "Alignment check",
    "Machine check",
    "SIMD Floating point",
    "Unknown Exception",
};
void DumpCPU(UINT32 ExceptionType, UINT32 EBX, UINT32 ECX, UINT32 EDX,        
	UINT16 ES, UINT16 SS, UINT16 GS,  UINT16 FS,  
	UINT32 EBP, UINT32 ESP, UINT32 ESI, UINT32 EDI, UINT32 EFlags, 
	UINT32 CR0, UINT32 CR1, UINT32 CR2, UINT32 CR3, UINT32 CR4, UINT16 DS, UINT32 EAX, UINT32 ErrorCode, UINT32 EIP, UINT32 CS )
{
    if ( ExceptionType >= TOTAL_EXCEPTIONS )
        ExceptionType = TOTAL_EXCEPTIONS - 1;
    
    printf(DumpCPUFormat, EAX, EBX, ECX, EDX, 
            CS, DS, ES, SS, GS, FS, 
            EBP, ESP, ESI, EDI, EIP,
            CR0, CR1, CR2, CR3, CR4,
            EFlags, ErrorCode, szExceptionType[ExceptionType], ExceptionType);
    dbgprintf(DumpCPUFormat, EAX, EBX, ECX, EDX, 
            CS, DS, ES, SS, GS, FS, 
            EBP, ESP, ESI, EDI, EIP,
            CR0, CR1, CR2, CR3, CR4,
            EFlags, ErrorCode, szExceptionType[ExceptionType], ExceptionType);
}

